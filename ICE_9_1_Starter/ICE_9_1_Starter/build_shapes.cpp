#include "build_shapes.hpp"
#include "stb_image_implementation.hpp"

BasicShape GetTriangle (VAOStruct vao,float size, glm::vec3 location) {
    float tri_vert[] {
        -1.0,-1.0,0.0,//bottom left vertex
        1.0,-1.0,0.0,  //bottom right vertex
        0.0,1.0,0.0  //top vertex
    };
    for (int i = 0; i < 3; i++) {
        tri_vert[3*i] = size * tri_vert[3*i] + location.x;
        tri_vert[3*i+1] = size * tri_vert[3*i+1] + location.y;
        tri_vert[3*i+2] = size * tri_vert[3*i+2] + location.z;
    }
    BasicShape new_shape;
    new_shape.Initialize(vao,tri_vert,sizeof(tri_vert),3,GL_TRIANGLES);

    unsigned int indices[] = {0,1,2};
    //Set the EBO for the shape here.
    new_shape.InitializeEBO(indices,sizeof(indices),3);
    return new_shape;
}

BasicShape GetTexturedTriangle (VAOStruct vao, glm::vec3 vertex_one, glm::vec3 vertex_two, glm::vec3 vertex_three) 
{
    float tri_vert[] {
        vertex_one.x, vertex_one.y, vertex_one.z, 0.0,0.0,
        vertex_two.x, vertex_two.y, vertex_two.z, 1.0,0.0,
        vertex_three.x, vertex_three.y, vertex_three.z, (vertex_three.x + 1.0f)/2.0f, 1.0
    };
    
    BasicShape new_shape;
    new_shape.Initialize(vao,tri_vert,sizeof(tri_vert),3,GL_TRIANGLES);

    unsigned int indices[] = {0,1,2};
    //Set the EBO for the shape here.
    new_shape.InitializeEBO(indices,sizeof(indices),3);
    return new_shape;
}

BasicShape GetRectangle (VAOStruct vao, glm::vec3 location, float width, float height)
{
    float vertices[] 
    {
        location.x, location.y, 0.0,
        location.x+width, location.y, 0.0,
        location.x+width, location.y+height,0.0,
        location.x, location.y+height, 0.0
    };
    BasicShape new_shape;
    new_shape.Initialize(vao,vertices,sizeof(vertices),4,GL_TRIANGLE_FAN);
    unsigned int indices[] = {0,1,2,3};
    new_shape.InitializeEBO(indices,sizeof(indices),4);
    return new_shape;

}

BasicShape GetTexturedRectangle (VAOStruct vao, glm::vec3 location, float width, float height)
{
    float vertices[] 
    {
        location.x, location.y, 0.0, 0.0, 0.0,
        location.x+width, location.y, 0.0, 1.0, 0.0,
        location.x+width, location.y+height,0.0, 1.0,1.0,
        location.x, location.y+height, 0.0,0.0,1.0
    };
    BasicShape new_shape;
    new_shape.Initialize(vao,vertices,sizeof(vertices),4,GL_TRIANGLE_FAN);
    unsigned int indices[] = {0,1,2,3};
    new_shape.InitializeEBO(indices,sizeof(indices),4);
    return new_shape;

}


BasicShape GetCircle (VAOStruct vao, float radius, int points, glm::vec3 location) {
    float circ_dat[points*3+3]{};
    float angle = 0.0;
    for(int i = 0; i<points; i++) {
        circ_dat[i*3] = location.x + radius*cos(glm::radians(angle));
        circ_dat[i*3+1] = location.y + radius*sin(glm::radians(angle));
        circ_dat[i*3+2] = 0.0f;
        angle+=360.0f/points;
    }
    
    circ_dat[points*3] = circ_dat[0];
    circ_dat[points*3+1] = circ_dat[1];
    circ_dat[points*3+2] = circ_dat[2];
    BasicShape new_shape;
    new_shape.Initialize(vao,circ_dat,sizeof(circ_dat),points*3,GL_TRIANGLE_FAN);

    //for use with the EBO
    unsigned int indices[points]{};
    for(int i = 0; i<points; i++)
    {
        indices[i] = i;
    }
    //set the ebo for the shape here
    new_shape.InitializeEBO(indices,sizeof(indices),points);
    return new_shape;
}

BasicShape GetTexturedCircle (VAOStruct vao, float radius, int points, glm::vec3 location) {
    float circ_dat[points*5]{};
    float angle = 0.0;
    for(int i = 0; i<points; i++) {
        circ_dat[i*5] = location.x + radius*cos(glm::radians(angle));
        circ_dat[i*5+1] = location.y + radius*sin(glm::radians(angle));
        circ_dat[i*5+2] = 0.0f;
        circ_dat[i*5+3] = 0.5 + (1+ cos(glm::radians(angle))/2);
        circ_dat[i*5+4] = 0.5 + (1+ sin(glm::radians(angle))/2);
        angle+=360.0f/points;
    }
    
    BasicShape new_shape;
    new_shape.Initialize(vao,circ_dat,sizeof(circ_dat),points,GL_TRIANGLE_FAN);

    //for use with the EBO
    unsigned int indices[points]{};
    for(int i = 0; i<points; i++)
    {
        indices[i] = i;
    }
    //set the ebo for the shape here
    new_shape.InitializeEBO(indices,sizeof(indices),points);
    return new_shape;
}


BasicShape GetHull (VAOStruct vao, glm::vec3 lower_left, float width, float height,bool use_texture)
{
float curve_height = 0.85;
float data[] = {
        lower_left.x, lower_left.y, 0.0f,
        lower_left.x+width, lower_left.y, 0.0f,
        lower_left.x+width, lower_left.y+height, 0.0f,
        lower_left.x+0.9f*width, lower_left.y+height+0.05f,0.0f, 
        lower_left.x+0.8f*width, lower_left.y+height+0.09f,0.0f, 
        lower_left.x+0.7f*width, lower_left.y+height+0.11f,0.0f, 
        lower_left.x+0.6f*width, lower_left.y+height+0.13f,0.0f, 
        lower_left.x+0.5f*width, lower_left.y+height+0.15f,0.0f,
        lower_left.x+0.4f*width, lower_left.y+height+0.13f,0.0f,
        lower_left.x+0.3f*width, lower_left.y+height+0.11f,0.0f,
        lower_left.x+0.2f*width, lower_left.y+height+0.09f,0.0f,
        lower_left.x+0.1f*width, lower_left.y+height+0.05f,0.0f,
        lower_left.x, lower_left.y+height, 0.0f
    };

    float textured_data[] = {
        lower_left.x, lower_left.y, 0.0f,0.0,0.0,
        lower_left.x+width, lower_left.y, 0.0f,1.0,0.0,
        lower_left.x+width, lower_left.y+height, 0.0f,1.0,curve_height,
        lower_left.x+0.9f*width, lower_left.y+height+0.05f,0.0f, 0.9,curve_height+0.05f,
        lower_left.x+0.8f*width, lower_left.y+height+0.09f,0.0f, 0.8,curve_height+0.09f,
        lower_left.x+0.7f*width, lower_left.y+height+0.11f,0.0f, 0.7,curve_height+0.11f,
        lower_left.x+0.6f*width, lower_left.y+height+0.13f,0.0f, 0.6,curve_height+0.13f,
        lower_left.x+0.5f*width, lower_left.y+height+0.15f,0.0f, 0.5, curve_height+0.15f,
        lower_left.x+0.4f*width, lower_left.y+height+0.13f,0.0f, 0.4, curve_height+0.13f,
        lower_left.x+0.3f*width, lower_left.y+height+0.11f,0.0f, 0.3, curve_height+0.11f,
        lower_left.x+0.2f*width, lower_left.y+height+0.09f,0.0f, 0.2, curve_height+0.09f,
        lower_left.x+0.1f*width, lower_left.y+height+0.05f,0.0f, 0.1, curve_height+0.05f,
        lower_left.x, lower_left.y+height, 0.0f,0.0, curve_height
    };

    BasicShape new_shape;
    if (!use_texture) 
    {
        new_shape.Initialize(vao,data,sizeof(data),sizeof(data)/sizeof(float)/3,GL_TRIANGLE_FAN);
        unsigned int indices[sizeof(data)/sizeof(float)/3];
        for (int i = 0; i<sizeof(data)/sizeof(float)/3; i++){indices[i] = i;}
        new_shape.InitializeEBO(indices,sizeof(indices),sizeof(data)/sizeof(float)/3);
    } 
    else 
    {
        new_shape.Initialize(vao,textured_data,sizeof(textured_data),sizeof(textured_data)/sizeof(float)/5,GL_TRIANGLE_FAN);
        unsigned int indices[sizeof(textured_data)/sizeof(float)/5];
        for (int i = 0; i<sizeof(textured_data)/sizeof(float)/5; i++){indices[i] = i;}
        new_shape.InitializeEBO(indices,sizeof(indices),sizeof(textured_data)/sizeof(float)/5);
    }
    
    
    return new_shape;

}

BasicShape GetStars (VAOStruct vao, int num_stars) {
    float data[num_stars*3] = {0.0f};
    for (int i = 0; i < num_stars; i+=3) {
        data[i] = 1.0f*(((rand() % 1000)-500)/500.0f);
        data[i+1] = 1.0f*(((rand()%1000)-500)/500.0f);
    }

    BasicShape new_shape;
    new_shape.Initialize(vao,data,sizeof(data),num_stars,GL_POINTS);
    return new_shape;
}

unsigned int GetTexture (std::string file_name) 
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &nrChannels, 0); 

    unsigned int texture;
    glGenTextures(1, &texture); 

    glBindTexture(GL_TEXTURE_2D, texture); 

    int channel_type = GL_RGB;
    if (nrChannels == 4) {channel_type = GL_RGBA;}

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, channel_type, width, height, 0, channel_type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}