-- Dialogue for NPC "npc_rana"
loadDialogue = function(DL) 

	if (not DL:isConditionFulfilled("npc_rana", "talked")) then 
		DL:setRoot(1) 
	else 
		DL:setRoot(2) 
	end 

	if (not DL:isConditionFulfilled("npc_rana", "talked")) then 

		DL:createNPCNode(1, 5, "DL_Rana_Hello") -- Hello there! You look like you're freezing. 
		DL:addConditionProgress("npc_rana", "talked")
		DL:addNode()


		DL:createNPCNode(5, -2, "DL_Rana_Hello2") -- Come and sit at our fire to warm you up. And why not have a little chat.
		DL:addNode()

	end


	DL:createChoiceNode(2)
	if (not DL:isConditionFulfilled("npc_rana", "who_are_you")) then 
		DL:addChoice(6, "DL_Choice_WhoAreYou") -- Who are you guys?
	end
	if (DL:isConditionFulfilled("npc_rana", "who_are_you") and not DL:isConditionFulfilled("npc_rana", "plans")) then 
		DL:addChoice(8, "DL_Choice_Plans") -- How's the hunt going?
	end
	if (DL:isConditionFulfilled("npc_rana", "who_are_you") and not DL:isConditionFulfilled("npc_rana", "yasha")) then 
		DL:addChoice(11, "DL_Choice_Yasha") -- Do you know Yasha?
	end
	if (not DL:isConditionFulfilled("npc_rana", "mages")) then 
		DL:addChoice(16, "DL_Choice_Mages") -- Have you seen some mages passing through here?
	end
	if (not DL:isConditionFulfilled("npc_rana", "trade")) then 
		DL:addChoice(4, "DL_Choice_UnlockTrade") -- Do you sell something?
	end
	if (DL:isConditionFulfilled("npc_rana", "trade")) then 
		DL:addChoice(3, "DL_Choice_Trade") -- Show me your wares.
	end
	DL:addChoice(-1, "") -- 
	DL:addNode()

	if (not DL:isConditionFulfilled("npc_rana", "who_are_you")) then 

		DL:createNPCNode(6, 7, "DL_Rana_WhoAreYou") -- Jason and I are demon hunters. We make sure that the demons of this valley stay here and don't make trouble.
		DL:addConditionProgress("npc_rana", "who_are_you")
		DL:addNode()


		DL:createNPCNode(7, -2, "DL_Rana_WhoAreYou2") -- We also hunt them for trophies like fur and teeth. They can be used to craft fine armour and the merchants in Gandria will buy them gladly.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_rana", "who_are_you") and not DL:isConditionFulfilled("npc_rana", "plans")) then 

		DL:createNPCNode(8, 9, "DL_Rana_Plans") -- Pretty well. We found some cat demons and even a gargoyle lately.
		DL:addConditionProgress("npc_rana", "plans")
		DL:addNode()


		DL:createNPCNode(9, 10, "DL_Rana_Plans2") -- But the really fat booty would be in that temple in the North.
		DL:addNode()


		DL:createNPCNode(10, -2, "DL_Rana_Plans3") -- Unfortunately, the spells guarding it are too strong and we can't get in. But there are still a lot of demons left in the ruins surrounding it.
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_rana", "who_are_you") and not DL:isConditionFulfilled("npc_rana", "yasha")) then 

		DL:createNPCNode(11, 12, "DL_Rana_Yasha") -- Yes, of course. She's guarding the valley and we wouldn't dare to fight with her. She's sleeping anyway at the moment.
		DL:addConditionProgress("npc_rana", "yasha")
		DL:addNode()


		DL:createChoiceNode(12)
		if (DL:isConditionFulfilled("npc_yasha", "unfriendly")) then 
			DL:addChoice(13, "DL_Choice_KilledYasha") -- I killed her.
		end
		if (not DL:isConditionFulfilled("npc_yasha", "unfriendly")) then 
			DL:addChoice(14, "DL_Choice_BefriendedYasha") -- She's not sleeping anymore, I made friends with her.
		end
		DL:addChoice(-2, "DL_Choice_Mhm") -- Mhm...
		DL:addNode()

		if (DL:isConditionFulfilled("npc_yasha", "unfriendly")) then 

			DL:createNPCNode(13, 15, "DL_Rana_KilledYasha") -- (Rana looks shocked) What? This can't be. And if it's true, you shouldn't have. 
			DL:addNode()


			DL:createNPCNode(15, -2, "DL_Rana_KilledYasha2") -- The other demons will be far more aggressive now. More work for us I guess. (Sighs)
			DL:addNode()

		end

		if (not DL:isConditionFulfilled("npc_yasha", "unfriendly")) then 

			DL:createNPCNode(14, -2, "DL_Rana_BefriendedYasha") -- Oh, really? Very interesting news. We won't do her any harm of course, she's like a goddess for this valley.
			DL:addNode()

		end

	end

	if (not DL:isConditionFulfilled("npc_rana", "mages")) then 

		DL:createNPCNode(16, 17, "DL_Rana_Mages") -- Hm, yes. There were four of them and they were heading for the old temple in the North.
		DL:addConditionProgress("npc_rana", "mages")
		DL:addNode()


		DL:createNPCNode(17, -2, "DL_Rana_Mages2") -- We offered them to escort them but their leader was very unfriendly. I don't know whether they made it.
		DL:addQuestDescription("find_velius", 2)
		DL:addNode()

	end

	if (not DL:isConditionFulfilled("npc_rana", "trade")) then 

		DL:createNPCNode(4, -2, "DL_Rana_UnlockTrade") -- Yes, I got some trophies and spare armour, if you're interested.
		DL:addConditionProgress("npc_rana", "trade")
		DL:addNode()

	end

	if (DL:isConditionFulfilled("npc_rana", "trade")) then 

		DL:createTradeNode(3, -2)
		DL:addNode()

	end

end