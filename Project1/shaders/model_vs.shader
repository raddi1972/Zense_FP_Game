#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

// TexCoord -> coordinates of texture to fragment
out vec2 TexCoords;

// mvp matrices to transform the coordinates
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// normal -> to get the normal for the current fragment
out vec3 normal;
// fragPos -> to get the coordinate of the fragment
out vec3 fragPos;

void main()
{
    // send the textures
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // sending the normal and the fragPos
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
}