#include <shape.hpp>

namespace opengles_workspace
{
    enum Direction
    {
        UP,
        LEFT,
        DOWN,
        RIGHT
    };
    enum Axis
    {
        VERTICAL,
        HORIZONTAL
    };

    class GameLogic
    {
    private:
        static Shape shapeMatrix[10][10];
        static int currentI;
        static int currentJ;
        static int score;

    public:
        GameLogic();
        ~GameLogic() {};

        static Shape GetShapeAt(int, int);
        static int GetScore();

        static void CheckShift(Shape&, Shape&, Direction);
        static void CalculateScore(Shape&, int, int);
        static void RandomizeCorrectShapes(int, int, int, int, Axis);

        static void Move(Direction);
        static void SelectShape();

    };
}