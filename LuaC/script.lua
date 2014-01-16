str_VertexShader = "#version 120\n" .. 
		"varying vec3 normal;\n" .. 
		"varying vec4 pos;\n" .. 
		"void main(void) {\n" .. 
		"  normal = gl_NormalMatrix * gl_Normal;\n" .. 
		"  pos = gl_ModelViewMatrix * gl_Vertex;\n" .. 
		"  gl_FrontColor = gl_Color;\n" ..
		"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n" .. 
		"}";
str_FragmentShader = "#version 120\n" ..
		"varying vec3 normal;\n" ..
		"varying vec4 pos;\n" ..
		"void main(void){\n" ..
		"  vec3 l_norm = normalize(\n" .. 
		"    vec3((gl_LightSource[0].position) - pos));\n" ..
		"  vec3 n_norm = normalize (normal);\n" ..
		"  float dot_n_l = dot (n_norm, l_norm);\n" ..
		"  float iDiff = max(0.0f, dot_n_l);\n" ..
		"  vec3 c_norm = normalize(-vec3(pos));\n" ..
		"  float iSpec = max(0.0f, pow(\n" ..
		"    dot(2 * dot_n_l * n_norm - l_norm, c_norm),\n" ..
		"    gl_FrontMaterial.shininess));\n" ..
		"  gl_FragColor = gl_FrontMaterial.emission + gl_Color *\n" ..
		"    (iDiff * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse +\n" ..
		"    iSpec * gl_FrontMaterial.specular * gl_LightSource[0].specular +\n" ..
		"    gl_FrontMaterial.ambient * gl_LightSource[0].ambient);\n" ..
		"}";

function InitializeLayout()
	SetEventHandler(OnGlobalEvent);
	RegisterEvent("EVENT_GLOBAL_LOG");

	--Command View
	TextField = CreateTextField();
	TextField:SetText("InstallAnotherShader()");
	TextField:SetPosition(10, 10);
	TextField:SetAnchor("ANCHOR_BOTTOM_LEFT", "ANCHOR_BOTTOM_LEFT");
	TextField:SetSize(360, 20);
	TextField:SetIsMultiLine(false);
	TextField:Show();
	TextField:SetEventHandler(OnEvent);
	TextField:RegisterEvent("EVENT_TEXT_SUBMIT");

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
	Label:SetPosition(0, 10);
	Label:SetAnchor("ANCHOR_BOTTOM_LEFT", "ANCHOR_TOP_LEFT", TextField);
	Label:Show();
	Label:SetText("");
	
	ButtonSwitch = CreateButton();
	ButtonSwitch:SetText("Switch to Vertex Shader");
	ButtonSwitch:SetPosition(-10, -10);
	ButtonSwitch:SetAnchor("ANCHOR_TOP_RIGHT", "ANCHOR_TOP_RIGHT");
	ButtonSwitch:SetSize(230, 20);
	ButtonSwitch:SetEventHandler(OnEvent);
	ButtonSwitch:RegisterEvent("EVENT_BUTTON_CLICK");
	ButtonSwitch:Show();

	-- Shader
	TextFieldShader = CreateTextField();
	TextFieldShader:SetText("#version 120");
	TextFieldShader:SetPosition(10, 40);
	TextFieldShader:SetAnchor("ANCHOR_BOTTOM_LEFT", "ANCHOR_BOTTOM_LEFT");
	TextFieldShader:SetSize(550, 500);
	TextFieldShader:SetIsMultiLine(true);
	TextFieldShader:SetEventHandler(OnEvent);
	TextFieldShader:RegisterEvent("EVENT_TEXT_SUBMIT");

	ButtonShader = CreateButton();
	ButtonShader:SetText("Vertex Shader Installieren");
	ButtonShader:SetPosition(0, -10);
	ButtonShader:SetAnchor("ANCHOR_TOP_RIGHT", "ANCHOR_BOTTOM_RIGHT", TextFieldShader);
	ButtonShader:SetSize(280, 20);
	ButtonShader:SetEventHandler(OnEvent);
	ButtonShader:RegisterEvent("EVENT_BUTTON_CLICK");
end

function OnEvent(source, event)
	--TextField:SetText(event);
	if (source == Button and event == "EVENT_BUTTON_CLICK") then
		--Button:SetText(TextField:GetText());
		local f, err = loadstring(TextField:GetText(), "ConsoleInput");
		if (f ~= nil) then
			f();
			TextField:SetText("");
		else
			--Label:SetText(err);
		end
	elseif (source == TextField and event == "EVENT_TEXT_SUBMIT") then
		local f, err = loadstring(TextField:GetText(), "ConsoleInput");
		if (f ~= nil) then
			f();
			TextField:SetText("");
		else
			--Label:SetText(err);
		end
	elseif (source == ButtonSwitch and event == "EVENT_BUTTON_CLICK" and ButtonSwitch:GetText() == "Switch to Vertex Shader") then
		ButtonSwitch:SetText("Switch to Fragment Shader");
		
		TextField:Hide();
		Label:Hide();
		Button:Hide();

		TextFieldShader:SetText(str_VertexShader);
		TextFieldShader:Show();
		ButtonShader:SetText("Vertex Shader Installieren");
		ButtonShader:Show();

	elseif (source == ButtonSwitch and event == "EVENT_BUTTON_CLICK" and ButtonSwitch:GetText() == "Switch to Fragment Shader") then
		ButtonSwitch:SetText("Switch to Command View");

		str_VertexShader = TextFieldShader:GetText();
		TextFieldShader:SetText(str_FragmentShader);
		ButtonShader:SetText("Fragment Shader Installieren");

	elseif (source == ButtonSwitch and event == "EVENT_BUTTON_CLICK" and ButtonSwitch:GetText() == "Switch to Command View") then
		ButtonSwitch:SetText("Switch to Vertex Shader");

		str_FragmentShader = TextFieldShader:GetText();
		TextFieldShader:Hide();
		ButtonShader:Hide();

		TextField:Show();
		Label:Show();
		Button:Show();
	elseif (source == ButtonShader and event == "EVENT_BUTTON_CLICK" and ButtonShader:GetText() == "Vertex Shader Installieren") then
		InstallVertexShader(TextFieldShader:GetText());
		InstallFragmentShader(str_FragmentShader);
	elseif (source == ButtonShader and event == "EVENT_BUTTON_CLICK" and ButtonShader:GetText() == "Fragment Shader Installieren") then
		InstallVertexShader(str_VertexShader);
		InstallFragmentShader(TextFieldShader:GetText());
	end
end

function OnGlobalEvent(event, arg1)
	if (event == "EVENT_GLOBAL_LOG") then
		Label:SetText(Label:GetText() .. arg1);
	end
end

InitializeLayout();

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

function InstallAnotherShader()
	InstallFragmentShader("#version 120\nvoid main(void){gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);}");
end
