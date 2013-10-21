--[[

Image
Button		// mit text, nicht scrollbar, zentriert // fertig implementiert
TextInput	// scrollbar, single/multiline, scroll on text input&mouse
TextOutput	// scrollbar, single/multiline, scroll on mouse




Text Scrollen
textfeld autovervollständigung


]]--

function InitializeLayout()
	TextField = CreateTextField();
	TextField:SetText("Dump(_G)");
	TextField:SetPosition(10, -10);
	TextField:SetAnchor("ANCHOR_BOTTOM_LEFT", "ANCHOR_BOTTOM_LEFT");
	--TextField:SetAnchor("ANCHOR_TOP_LEFT", "ANCHOR_TOP_LEFT");
	--TextField:SetAnchor("ANCHOR_BOTTOM_RIGHT", "ANCHOR_BOTTOM_RIGHT");
	--TextField:SetAnchor("ANCHOR_TOP_RIGHT", "ANCHOR_TOP_RIGHT");
	TextField:SetSize(360, 20);
	TextField:Show();
	--TextField:RegisterEvent("EVENT_TEXT_SUBMIT", OnEvent);

	Button = CreateButton();
	Button:SetText("Ausführen");
	Button:SetPosition(10, 0);
	Button:SetAnchor("ANCHOR_TOP_LEFT", "ANCHOR_TOP_RIGHT", TextField);
	Button:SetSize(80, 20);
	Button:SetEventHandler(OnEvent);
	Button:RegisterEvent("EVENT_BUTTON_CLICK");
	Button:Show();

	
	Label = CreateLabel();
	Label:SetSize(450, 200);
	Label:SetPosition(0, -10);
	Label:SetAnchor("ANCHOR_BOTTOM_LEFT", "ANCHOR_TOP_LEFT", TextField);
	Label:Show();
	Label:SetText("");
	--Label:RegisterEvent(EVENT_TEXT_SUBMIT, OnEvent);
	
	RegisterEvent("EVENT_GLOBAL_LOG", OnGlobalEvent);

end


function OnEvent(source, event)
	--TextField:SetText(event);
	if (source == Button and event == "EVENT_BUTTON_CLICK") then
		--Button:SetText(TextField:GetText());
		local f, err = loadstring(TextField:GetText(), "ConsoleInput");
		if (f ~= nil) then
			f();
		else
			--Label:SetText(err);
		end
	end
end

InitializeLayout();


function OnGlobalEvent(event, arg1)
	if (event == EVENT_GLOBAL_LOG) then
		Label:AppentText(arg1);
	end
end

function Dump(t)
	local str = "";

	if (type(t) == "string") then
		str = t
	elseif (type(t) == "table") then
		for k,v in pairs(t) do 
		
			str = str .. k .. "("..type(v).."):" .. tostring(v) .. "\n";
		end
	end
	if (Label:GetText() == "") then
		Label:SetText(Label:GetText() .. "Content: \n" .. str);
	else
		Label:SetText(Label:GetText() .. "\nContent: \n" .. str);
	end

end



--[[
Milestones:
1. 24.10.
2. 03.12.
3. 16.01.

z = "Hallo Welt"; Dump(z)

]]--
