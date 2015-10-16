#include "FileIO/CSVReader.h"
#include "ResourceManager.h"

using namespace std;

#define CSV_SEP ';'

// Returns a pointer to the start of the next field, or zero if this is the
// last field in the CSV p is the start position of the field sep is the
// separator used, i.e. comma or semicolon newline says whether the field ends
// with a newline or with a comma
const char* CSVReader::nextCsvField(const char *p, bool *newline, const char **escapedEnd) const {
	*escapedEnd = nullptr;
	*newline = false;

	// Parse quoted sequences
	if ('"' == p[0]) {
		p++;
		while (1) {
			// Find next double-quote
			p = strchr(p, '"');
			// Check for "", it is an escaped double-quote
			if (p[1] != '"') {
				*escapedEnd = p;
				break;
			}
			// If we don't find it or it's the last symbol
			// then this is the last field
			if (!p || !p[1])
				return nullptr;
			// Skip the escaped double-quote
			p += 2;
		}
	}

	// Find next newline or comma.
	char newline_or_sep[4] = "\n\r ";
	newline_or_sep[2] = CSV_SEP;
	p = strpbrk(p, newline_or_sep);

	// If no newline or separator, this is the last field.
	if (!p)
		return nullptr;

	// Check if we had newline.
	*newline = (p[0] == '\r' || p[0] == '\n');

	// Handle "\r\n", otherwise just increment
	if (p[0] == '\r' && p[1] == '\n')
		p += 2;
	else
		p++;

	return p;
}

// Parses the CSV data and constructs a StringTable
// from the fields in it.
void CSVReader::parseCsv(const char *csvData, StringTable& table) const {
	table.clear();

	// Return immediately if the CSV data is empty.
	if (!csvData || !csvData[0]) {
		return;
	}

	table.resize(1);

	// Here we CSV fields and fill the output StringTable.
	while (csvData) {
		// Call nextCsvField.
		bool newline;
		const char *escapedEnd;
		const char *next = nextCsvField(csvData, &newline, &escapedEnd);

		// Add new field to the current row.
		table.back().resize(table.back().size() + 1);
		std::string &field = table.back().back();

		// If there is a part that is escaped with double-quotes, add it
		// (without the opening and closing double-quote, and also with any ""
		// escaped to ". After that csvData is set to the part immediately
		// after the closing double-quote, so anything after the closing
		// double-quote is added as well (but unescaped).
		if (escapedEnd) {
			for (const char *ii = csvData + 1; ii != escapedEnd; ii++) {
				field += *ii;
				if ('"' == ii[0] && '"' == ii[1])
					ii++;
			}
			csvData = escapedEnd + 1;
		}

		// If there was no escaped part, or the CSV is malformed, add anything
		// else "as is" (i.e. unescaped). Keep in mind that next might be NULL.
		if (next)
			field.append(csvData, next - 1);
		else
			field += csvData;

		// If the field ends with a newline, add next row to the StringTable.
		if (newline) {
			if (field.empty()) {
				table.back().pop_back();
			}
			table.resize(table.size() + 1);
		}

		// Set csvData to point to the start of the next field for the next
		// cycle.
		csvData = next;
	}

	// If the CSV ends with a newline, then there is an empty row added
	// (actually it's a row with a single empty string). We trim that empty
	// row here.
	if (table.back().empty() || (table.back().size() == 1 && table.back().front().empty())) {
		table.pop_back();
	}
}