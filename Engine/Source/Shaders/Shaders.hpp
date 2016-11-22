#pragma once
#include<map>
#include<string>
namespace vh {
  const std::map<std::string, std::string> Shaders = {{"OSD_FS.glsl","\nin lowp vec2 fTexCoord;\n\nout lowp vec4 color;\n\nuniform sampler2D uSampler;\n\nvoid main() {\n color = texture(uSampler, fTexCoord);\n}\n"},{"OSD_VS.glsl","\nlayout(location = 0) in vec2 iVertexPos;\n\nuniform vec4 uBounds; \nout vec2 fTexCoord;\n\nvoid main() {\n gl_Position.xy = iVertexPos;\n gl_Position.z = -0.999999;\n gl_Position.w = 1.0;\n fTexCoord = vec2(\n (iVertexPos.x - uBounds.x) / uBounds.z,\n (- iVertexPos.y + uBounds.y) / uBounds.w\n );\n }\n"},{"SimpleShader_FS.glsl","\nconst int MAX_PLIGHTS = 10;\n\nin lowp vec2 fUV;\n\nin lowp vec3 fAmbientColor;\nin lowp vec3 fDiffuseColor;\nin lowp vec3 fSpecularColor;\n\nin lowp vec3 fNormal_cameraspace;\nin lowp vec3 fPLightDir_cameraspace[MAX_PLIGHTS];\n\nin lowp vec3 fEyeDirection_cameraspace;\nin lowp float fLightDistance;\n\nout lowp vec3 color;\n\nuniform int uPLightNum; uniform float uPLightInt[MAX_PLIGHTS]; uniform sampler2D textureSampler;\n\nvoid main() {\n lowp vec3 n = normalize(fNormal_cameraspace);\n lowp vec3 E = normalize(fEyeDirection_cameraspace);\n\n vec3 diffuse = vec3(0);\n vec3 specular = vec3(0);\n\n for (int i = 0; i < uPLightNum; i++)\n {\n lowp vec3 l = normalize(fPLightDir_cameraspace[i]);\n lowp vec3 R = reflect(- l, n);\n\n lowp float cosAlpha = clamp(dot(E, R), 0, 1);\n lowp float cosTheta = clamp(dot(n, l), 0, 1);\n\n diffuse += (fDiffuseColor + texture(textureSampler, fUV).rgb) * cosTheta * uPLightInt[i];\n\n specular += fSpecularColor * pow(cosAlpha, 5) * uPLightInt[i];\n }\n\n vec3 ambient = fAmbientColor * 0.1; \n\n color = ambient + diffuse + specular;\n}\n"},{"SimpleShader_VS.glsl","\nconst int MAX_PLIGHTS = 10;\n\nlayout(location = 0) in vec3 iVertexPos;\nlayout(location = 1) in vec3 iVertexNormal;\n\nlayout(location = 2) in vec3 iAmbientColor;\nlayout(location = 3) in vec3 iDiffuseColor;\nlayout(location = 4) in vec3 iSpecularColor;\n\nlayout(location = 5) in vec2 iUV;\n\nout vec3 fAmbientColor;\nout vec3 fDiffuseColor;\nout vec3 fSpecularColor;\n\nout vec3 fNormal_cameraspace;\nout vec3 fPLightDir_cameraspace[MAX_PLIGHTS];\n\nout vec3 fEyeDirection_cameraspace;\n\nout vec2 fUV;\n\nuniform mat4 uMVP;\nuniform mat4 uM;\nuniform mat4 uV;\n\nuniform int uPLightNum; uniform vec3 uPLightPos[MAX_PLIGHTS]; uniform float uPLightInt[MAX_PLIGHTS]; \nvoid main() {\n gl_Position = uMVP * vec4(iVertexPos, 1);\n\n vec3 position_worldspace = (uM * vec4(iVertexPos, 1)).xyz;\n\n vec3 iVertexPos_cameraspace = (uV * uM * vec4(iVertexPos, 1)).xyz;\n fEyeDirection_cameraspace = vec3(0) - iVertexPos_cameraspace;\n\n fNormal_cameraspace = (uV * uM * vec4(iVertexNormal, 0)).xyz;\n\n fAmbientColor = iAmbientColor;\n fDiffuseColor = iDiffuseColor;\n fSpecularColor = iSpecularColor;\n\n for (int i = 0; i < uPLightNum; i++)\n {\n fPLightDir_cameraspace[i] = (uV * vec4(uPLightPos[i], 1)).xyz + fEyeDirection_cameraspace;\n }\n\n fUV = iUV;\n}\n"},{"SkyDome_FS.glsl","\nin lowp vec3 fTexCoord;\n\nout lowp vec3 color;\n\nuniform samplerCube uSkySampler;\n\nvoid main() {\n color = texture(uSkySampler, fTexCoord).rgb;\n}\n"},{"SkyDome_VS.glsl","\nlayout(location = 0) in vec3 iVertexPos;\n\nout vec3 fTexCoord;\n\nuniform mat4 uMVP;\n\nvoid main() {\n gl_Position = uMVP * vec4(iVertexPos, 1.0);\n fTexCoord = iVertexPos;\n}\n"},{"Wireframe_FS.glsl","\nin lowp vec3 fColor;\n\nout lowp vec4 color;\n\nvoid main() {\n color = vec4(fColor, 1.0);\n}\n"},{"Wireframe_VS.glsl","\nlayout(location = 0) in vec3 iVertexPos;\n\nuniform vec3 uColor;\n\nuniform mat4 uMVP;\n\nout vec3 fColor;\n\nvoid main() {\n gl_Position = uMVP * vec4(iVertexPos, 1);\n gl_Position.z = 1.0;\n fColor = uColor;\n}\n"},};
}