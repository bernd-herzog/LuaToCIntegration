--print("Hello from Lua")
--SetGodMode(true);


function InitializeLayout()
	TextField = CreateTextField();
	TextField:SetText("Hello World");
	TextField:SetPosition(0.1, 0.1);
	TextField:SetSize(0.4, 0.4);
	TextField:Show();
	TextField:RegisterEvent(EVENT_TEXT_SUBMIT, OnEvent);


	Label = CreateLabel();
	Label:SetPosition(0.1, 0.5);
	Label:SetSize(0.4, 0.4);
	Label:Show();
	Label:SetText("Hello World");
	--Label:RegisterEvent(EVENT_TEXT_SUBMIT, OnEvent);

	RegisterEvent(EVENT_GLOBAL_LOG, OnGlobalEvent);

end

function OnEvent(source, event)
	if (event == EVENT_TEXT_SUBMIT) then
		local f = loadstring(TextField:GetText())
		f();
	end
end

function OnGlobalEvent(event, arg1)
	if (event == EVENT_GLOBAL_LOG) then
		Label:AppentText(arg1);
	end
end

InitializeLayout();




--[[
Milestones:
1. 24.10.
2. 03.12.
3. 16.01.

Button
Text input
Bild
Text

]]--
--sfd();