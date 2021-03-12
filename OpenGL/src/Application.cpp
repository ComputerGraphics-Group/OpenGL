#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

///
/// Struct to hold the vertex shader and fragment shader as strings
///
struct ShaderProgramSource
{
    std::string vertex_source;
    std::string fragment_source;
};

///
/// Struct to hold a vector attributes, i.e., i and j
///
struct Vec
{
    float i;
    float j;
};

struct Color
{
    int x;
    int y;
};

///
/// Function to read the shaders in the file and return ShaderProgramSource
///
static ShaderProgramSource parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

///
/// Compiles a shader component and returns its id
///
static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

///
/// Creates a complete shader and returns its id
///
static unsigned int createShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

///
/// Bresenham's line drawing algorithm
///
Color makeLine(std::vector<float>& points, int x1, int y1, int x2, int y2, bool is_vector, Color c)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int i, d;
	int incNE, incE;
	int x, y;
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;
	int incx = 1;
	if (x2 < x1)
		incx = -1;
	int incy = 1;
	if (y2 < y1)
		incy = -1;
	x = x1; y = y1;
	if (dx > dy)
	{
		points.push_back(x);
		points.push_back(y);
		if (is_vector)
		{
			points.push_back(dx);
			points.push_back(dy);
		}
		else
		{
			points.push_back(c.x);
			points.push_back(c.y);
		}
		d = 2 * dy - dx;
		incNE = 2 * (dy - dx);
		incE = 2 * dy;
		for (i = 0; i < dx; i++)
		{
			if (d >= 0)
			{
				y += incy;
				d += incNE;
			}
			else
				d += incE;
			x += incx;
			points.push_back(x);
			points.push_back(y);
			if (is_vector)
			{
				points.push_back(dx);
				points.push_back(dy);
			}
			else
			{
				points.push_back(c.x);
				points.push_back(c.y);
			}
		}
	}
	else
	{
		points.push_back(x);
		points.push_back(y);
		if (is_vector)
		{
			points.push_back(dx);
			points.push_back(dy);
		}
		else
		{
			points.push_back(c.x);
			points.push_back(c.y);
		}
		d = 2 * dx - dy;
		incNE = 2 * (dx - dy);
		incE = 2 * dx;
		for (i = 0; i < dy; i++)
		{
			if (d >= 0)
			{
				x += incx;
				d += incNE;
			}
			else
				d += incE;
			y += incy;
			points.push_back(x);
			points.push_back(y);
			if (is_vector)
			{
				points.push_back(dx);
				points.push_back(dy);
			}
			else
			{
				points.push_back(c.x);
				points.push_back(c.y);
			}
		}
	}
	return { dx, dy };
}

///
/// Bresenham's circle drawing algorithm
///
void makeCircle(std::vector<float>& points, int xc, int yc, int r, Color c)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int deltaE = 3;
    int deltaSE = -2 * r + 5;
    points.push_back(x + xc);
    points.push_back(y + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(-x + xc);
    points.push_back(y + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(x + xc);
    points.push_back(-y + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(-x + xc);
    points.push_back(-y + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(y + xc);
    points.push_back(x + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(-y + xc);
    points.push_back(x + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(y + xc);
    points.push_back(-x + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    points.push_back(-y + xc);
    points.push_back(-x + yc);
    points.push_back(c.x);
    points.push_back(c.y);

    while (y > x)
    {
        if (d < 0)
        {
            d = d + deltaE;
            deltaE = deltaE + 2;
            deltaSE = deltaSE + 2;
        }
        else
        {
            d = d + deltaSE;
            deltaE = deltaE + 2;
            deltaSE = deltaSE + 4;
            y = y - 1;
        }
        x = x + 1;
        points.push_back(x + xc);
        points.push_back(y + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(-x + xc);
        points.push_back(y + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(x + xc);
        points.push_back(-y + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(-x + xc);
        points.push_back(-y + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(y + xc);
        points.push_back(x + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(-y + xc);
        points.push_back(x + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(y + xc);
        points.push_back(-x + yc);
        points.push_back(c.x);
        points.push_back(c.y);

        points.push_back(-y + xc);
        points.push_back(-x + yc);
        points.push_back(c.x);
        points.push_back(c.y);
    }
}

///
/// Computes the inverse square root of a float
///
float invSqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;            // store floating-point bits in integer
    i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x * (1.5f - xhalf * x * x);     // One round of Newton's method
    return x;
}

///
/// Makes an arrow head by calculating the 3 points and calling makeLine() on each pair
///

void makeArrowHead(std::vector<float>& points, int x1, int y1, Vec vector, Color c)
{
    float normalization_factor = invSqrt(vector.i * vector.i + vector.j * vector.j);
    float normalized_i = 5 * vector.i * normalization_factor;
    float normalized_j = 5 * vector.j * normalization_factor;
    makeLine(points, x1 + vector.i, y1 + vector.j, x1 + vector.i - normalized_j - normalized_i, y1 + vector.j + normalized_i - normalized_j, false, c);
    makeLine(points, x1 + vector.i, y1 + vector.j, x1 + vector.i - normalized_i + normalized_j, y1 + vector.j - normalized_i - normalized_j, false, c);
    makeLine(points, x1 + vector.i - normalized_j - normalized_i, y1 + vector.j + normalized_i - normalized_j, x1 + vector.i - normalized_i + normalized_j, y1 + vector.j - normalized_i - normalized_j, false, c);
}
///
/// The vector field function
///
Vec vectorField(float x, float y)
{
    return { x*x*y+y*x*x, x*x*y-y*y*x };
}

float scale = 0.000001;
float sparsity = 50;
float zoom = 1;
bool drawn = false;

///
/// Mouse scroll callback. Scrolling up/down increses/decreases the zoom
///
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0)
    {
        zoom *= 1.1;
        sparsity *= 1.1;
    }
    else if (yoffset < 0)
    {
        zoom /= 1.1;
        sparsity /= 1.1;
    }
    drawn = false;
}

///
/// Mouse button callback. Clicking right/left increses/decreases the sparsity
///
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (sparsity > 5 && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        sparsity *= 1.1;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        sparsity /= 1.1;
    drawn = false;
}

///
/// Keyboard key callback. Pressing S will reset the sparsity and pressing Z will reset the zoom
///
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        sparsity = 50;
    else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        zoom = 1;
    drawn = false;
}

///
/// Main function. Subienay you do this ask me if you have any problems
///
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 1000, "Bresenham's line drawing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    std::vector<float> points;

    Vec vector;

    float* points_array;         //this is for passing the vector as a pointer to the buffer data. Eventually this will be set to &points[0]

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(sizeof(float) * 2));

    ShaderProgramSource source = parseShader("res/shaders/basic.shader");
    unsigned int shader = createShader(source.vertex_source, source.fragment_source);
    glUseProgram(shader);

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    double tpp;
    double sum_tpp=0;
    int n=0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        if (!drawn)
        {
            n++;
            points.clear();
            //The drawwing starts:
            std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
            Color c;
            for (int i = -500; i < 501; i = i + sparsity)
            {
                for (int j = -500; j < 501; j = j + sparsity)
                {
                    vector = vectorField(i * 1 / zoom, j * 1 / zoom);     //If zoom is 2, then we have to find the vector not at (i,j) but at (i/2,j/2) and then plot it at (i,j)
                    //           makeCircle(points, i, j, sqrt(vector.i * zoom * scale * vector.i * zoom * scale + vector.j * zoom * scale * vector.j * zoom * scale));
                    c = makeLine(points, i, j, i + vector.i * zoom * scale, j + vector.j * zoom * scale, true, { 0,0 });     //We don't want to plot from (i,j) to (i+vector.i, j+vector.j) cause that would be massive hence the scale. Also as zoom increases, the vectors should appear bigger
                    makeCircle(points, i, j, 2, c); //teeny tiny circle at base for aesthetic
                    makeArrowHead(points, i, j, { (vector.i * zoom * scale), (vector.j * zoom * scale) }, c); //make an arrow head
                }
            }

            for (int i = 0; i < points.size(); i++)     //normalizing points
                points.at(i) = (points.at(i) * 0.002);
            points_array = &points[0];  //the promised step

            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points_array, GL_STATIC_DRAW);

            drawn = true;
            std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
            std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            std::cout << "Number of points: " << points.size() / 4 << std::endl;
            std::cout << "Duration to compute: " << duration.count() << std::endl;
            tpp = (double)duration.count() / (double)points.size();
            sum_tpp += tpp;
            std::cout << "Time per point: " << tpp << std::endl;
            std::cout << "Average time per point so far: " << sum_tpp/(double)n << std::endl << std::endl;
        }

        glDrawArrays(GL_POINTS, 0, points.size() / 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}