#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>


#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> sfera(int nMeridiane, int nParalele, float R);
GLuint textura(char fisier[]);

// configurari
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 700;

const unsigned int noParalele = 20;
const unsigned int noMeridiane = 20;
const float razaSferei = 1.0;
//const float razaCalotei = 0.9;


std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath;
        std::cerr << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}



int main()
{
    // glfw: initializare si configurare

    glfwInit();
    // precizam versiunea 3.3 de openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw cream fereastra

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sistem solar", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // facem ca aceasta fereastra sa fie contextul curent

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: incarcam referintele la functiile OpenGL


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // incarcam si compilam shaderele:

    // vertex shader

    glEnable(GL_DEPTH_TEST);

    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    if( 0 == vertexShader )
    {
        std::cout << "Error creating vertex shader." << std::endl;
        exit(1);
    }

    std::string shaderCode = readFile("sfera.vert");
    const char *codeArray = shaderCode.c_str();
    glShaderSource( vertexShader, 1, &codeArray, NULL );

    glCompileShader(vertexShader);

    // verficam daca s-a reusit compilarea codului

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader (repetam aceleasi operatii)

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    shaderCode = readFile("basic.frag");
    codeArray = shaderCode.c_str();
    glShaderSource( fragmentShader, 1, &codeArray, NULL );


    glCompileShader(fragmentShader);

    // se verifica compilarea codului

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // se verifica procesul de link

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    ///schimbari
    GLuint textura_earth=textura("texturi/earth.bmp");
    //glUniform1i(glGetUniformLocation(shaderProgram,"textura_mea"),0);

    GLuint textura_sun=textura("texturi/sun.jpg");
    //glUniform1i(glGetUniformLocation(shaderProgram,"textura_mea"),0);
    GLuint textura_moon=textura("texturi/moon.jpg");


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // initializam punctele de pe calota sferica
    std::vector<float> vertices;
    vertices = sfera(noMeridiane, noParalele, razaSferei);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // se face bind a obiectului Vertex Array, apoi se face bind si se stabilesc
    // vertex buffer(ele), si apoi se configureaza vertex attributes.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),  &vertices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);                     ///schimbare
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));     ///schimbare
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);                       ///schimbare

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // se face unbind pentru VAO
    glBindVertexArray(0);

    // ciclu de desenare -- render loop

    //definim cu cat rotim planeteler
    float step = 0.01, angle = 0;


    while (!glfwWindowShouldClose(window))
    {
        // input

        processInput(window);

        // render

        // space color
        glClearColor(0.0f, 0.0f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // specificam programul ce trebuie folosit
        glUseProgram(shaderProgram);

        // cream transformarile care ne definesc modul in care privim obiectul
        glm::mat4 model = glm::mat4(1.0f); /// this is the earth
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // calculate the new angle
        angle = angle + step;
        if (angle > 360)
            angle = angle - 360;

        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        /// move earth to where it should be and rotate around its center
        model  = glm::translate(model, glm::vec3(glm::sin(angle)* 6, 0.0f, glm::cos(angle)* 10));
        model = glm::rotate(model, glm::radians(angle * -100), glm::vec3(0.0f, 1.0f, 0.0f));

        /// rotate it so that it is with the north pole pointing up and a bit to the side
        model = glm::rotate(model, glm::radians(23.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        view  = glm::translate(view,glm::vec3(0.0f, 0.0f, -25.0f));
        projection = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // obtinem locatiile variabilelor uniforms in program
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        // transmitem valorile lor catre shadere (2 metode)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        // specificam modul in care vrem sa desenam -- aici din spate in fata, si doar contur
        // implicit pune fete, dar cum nu avem lumini si umbre deocamdata cubul nu va arata bine
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, textura_earth);


        //glBindVertexArray(VAO);
        //glBindTexture(GL_TEXTURE_2D, textura_sun);
        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices.size()/5);// vertices.size()/3-12, 12);//          ///schimbare

        /// The Moon
        model= glm::mat4(1.0f);
        /// Move it in the same place we move the Earth
        model = glm::translate(model, glm::vec3(glm::sin(angle)* 6, 0.0f, glm::cos(angle)* 10));
        /// Rotate it and move it away from the Earth
        model = glm::rotate(model, glm::radians(angle * 80), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
        /// Also make it smaller
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

        // set the Moon's model transformation in the shader program
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // bind the Moon's texture and draw it
        glBindTexture(GL_TEXTURE_2D, textura_moon);
        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices.size() / 5);

        /// The Sun
        model= glm::mat4(1.0f);
        /// Slowly rotate it as well
        model = glm::rotate(model, glm::radians(angle*-100), glm::vec3(0.0f, 1.0f, 0.0f));
        /// Also rotate 90 cuz it looks better idk
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

        // send info to shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // bind texture and draw
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, textura_sun);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size()/5);

        // glfw: se inverseaza zonele tamponm si se trateaza evenimentele IO

        glfwSwapBuffers(window);
        Sleep(20);
        glfwPollEvents();
    }


    // optional: se elibereaza resursele alocate

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteProgram(shaderProgram);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // glfw: se termina procesul eliberand toate resursele alocate de GLFW

    glfwTerminate();
    return 0;
}

// se proceseaza inputurile de la utilizator

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ce se intampla la o redimensionalizare a ferestrei

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ne asiguram ca viewportul este in concordanta cu noile dimensiuni

    glViewport(0, 0, width, height);
}

std::vector <float> sfera(int nMeridiane, int nParalele, float R){

    std::vector <float> vertex = {};
    std::cout<< 'fgds'<<vertex.size()<<std::endl;
    //float alpha = asin(R);
    double theta0, theta1, phi;
    int i,j;
    for(i = 1; i <= nParalele ; i++){

        theta0 =  M_PI/2- M_PI * ( (double) (i - 1) / nParalele);
        theta1 = M_PI/2- M_PI * ( (double) i / nParalele);

        for(j = 0; j <= nMeridiane; j++) {
            phi = 2 * M_PI * (double) (j-1) / nMeridiane;

            vertex.push_back(R * cos(theta0) * cos(phi));
            vertex.push_back(R * cos(theta0) * sin(phi));
            vertex.push_back(R * sin(theta0));
            vertex.push_back((i-1)/(nParalele+1.0));    ///+1 il facem real
            vertex.push_back((j)/(nMeridiane+1.0));

           vertex.push_back(R * cos(theta1) * cos(phi));
           vertex.push_back(R * cos(theta1) * sin(phi));
           vertex.push_back(R * sin(theta1));
            vertex.push_back((i)/(nParalele+1.0));    ///+1 il facem real
            vertex.push_back((j)/(nMeridiane+1.0));

            }
    }

    return vertex;

}

GLuint textura(char fisier[]){
    /*
    functia textura -- creaza un obiect tip textura (varianta simplificata)
    In:
        fisier - fisierul imagine ce contine textura
    Out:
        aliasul (numele) obiectului tip textura incarcat

    Exemplu de utilizare:
        In programul principal:
            _se defineste o variabila GLuint care va lua valori rezultatul acestei functii
            _se precizeaza ce sampler ii corespunde texturii noastre (se face doar odata)
                    glUniform1i(glGetUniformLocation(program, "variabila_textura"), 0);
            _inainte de a desena elementele se face bind pe textura dorita
                    glBindTexture(GL_TEXTURE_2D, alias_textura);

        In fragment shader:
            _definim variabila uniforms variabila_textura
                uniform sampler2D variabila_textura;

            _in functia main vom apela ceva de genul:
                FragColor = texture(variabila_textura, TexCoord);
            unde TexCoord va fi coordonatele textelilor

    */
    GLuint rezTextura;
    int width, height, nrChannels;

    // cream un obiect de tip textura
    glGenTextures(1, &rezTextura);

    // il legam de contextul curent
    glBindTexture(GL_TEXTURE_2D, rezTextura);

    // definim parametrii ei
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // definim parametrii de filtrare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps

    //stbi_set_flip_vertically_on_load(true); // facem un flip pe axa Oy

    // incarcam datele din fisierul imagine
    unsigned char *data = stbi_load(fisier, &width, &height, &nrChannels, 0);
    if (data)
    {
        //transferam datele incarcate in obiectul textura
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // generam mipmap -uri pentru aceasta textura
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    //eliberam datele incarcate din fisierul imagine
    stbi_image_free(data);

    //returnam aliasul texturii create
    return rezTextura;

}
