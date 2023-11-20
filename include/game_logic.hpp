#include <shape.hpp>

 #ifndef gamelogic
 #define gamelogic
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
    public:
        const static int gameBoardSize = 9;

    private:
        static Shape shapeMatrix[gameBoardSize][gameBoardSize];
        static int currentI;
        static int currentJ;
        static int score;
        static bool isSomethingSelected;

    public:
        GameLogic();
        ~GameLogic() {};

        static Shape GetShapeAt(int, int);
        static int GetCurrentI();
        static int GetCurrentJ();
        static int GetScore();
        static bool GetSomethingSelectedFlag();

        static void CheckShift(Shape&, Shape&, Direction);
        static void CalculateScore(Shape&, int, int);
        static void RandomizeCorrectShapes(int, int, int, int, Axis);

        static void Move(Direction);
        static void SelectShape();
    };
}
#endif