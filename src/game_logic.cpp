#include <game_logic.hpp>
#include <stdio.h>

namespace opengles_workspace
{
    const int GameLogic::gameBoardSize;
    Shape GameLogic::shapeMatrix[gameBoardSize][gameBoardSize];
    int GameLogic::currentI = 0;
    int GameLogic::currentJ = 0;
    int GameLogic::score = 0;
    bool GameLogic::isSomethingSelected = false;

    #define currentShape    shapeMatrix[currentI][currentJ]
    #define shapeUp         shapeMatrix[currentI - 1][currentJ]
    #define shapeLeft       shapeMatrix[currentI][currentJ - 1]
    #define shapeDown       shapeMatrix[currentI + 1][currentJ]
    #define shapeRight      shapeMatrix[currentI][currentJ + 1]

    GameLogic::GameLogic()
    {
        for (int i = 0; i < gameBoardSize; i++)
        {
            for (int j = 0; j < gameBoardSize; j++)
            {
                shapeMatrix[i][j].SetRandomColour();
            }
        }
    }

    /// @brief Get shape at desired indexes
    /// @param i first index
    /// @param j second index
    /// @return shapeMatrix[i][j]
    Shape GameLogic::GetShapeAt(int i, int j)
    {
        return shapeMatrix[i][j];
    }

    /// @brief Get current shape I index
    /// @return currentI
    int GameLogic::GetCurrentI()
    {
        return currentI;
    }

    /// @brief Get current shape J index
    /// @return currentJ
    int GameLogic::GetCurrentJ()
    {
        return currentJ;
    }

    /// @brief Get current game score
    /// @return score
    int GameLogic::GetScore()
    {
        return score;
    }

    /// @brief Get flag for any shape being currently selected
    /// @return isSomethingSelected (true = any shape is selected, false = no shape is selected)
    bool GameLogic::GetSomethingSelectedFlag()
    {
        return isSomethingSelected;
    }

    /// @brief Check a shift made between two shapes
    /// @param firstShape first shape to check
    /// @param secondShape second shape to check
    /// @param direction direction the shift was made in
    void GameLogic::CheckShift(Shape& firstShape, Shape& secondShape, Direction direction)
    {
        int firstI = currentI;
        int firstJ = currentJ;
        int secondI, secondJ;
        switch (direction)
        {
        case UP:
            secondI = currentI - 1;
            secondJ = currentJ;
            break;
        case LEFT:
            secondI = currentI;
            secondJ = currentJ - 1;
            break;
        case DOWN:
            secondI = currentI + 1;
            secondJ = currentJ;
            break;
        case RIGHT:
            secondI = currentI;
            secondJ = currentJ + 1;
            break;
        default:
            break;
        }
        printf("Check shift --- %s[%d][%d] & %s[%d][%d]\n",
                                                               firstShape.GetColourAsString(), firstI, firstJ,
                                                               secondShape.GetColourAsString(), secondI, secondJ);

        // Calculate for first shape
        CalculateScore(firstShape, firstI, firstJ);

        // Calculate for second shape
        CalculateScore(secondShape, secondI, secondJ);

        printf("Current score: %d\n", score);
    }

    /// @brief Verify all four direction for a shape to check possible matches and calculate score
    /// @param initialShape shape to check
    /// @param I shape index i
    /// @param J shape index j
    void GameLogic::CalculateScore(Shape& initialShape, int I, int J)
    {
        printf("Calculate score --- %s[%d][%d]\n",
                                                  initialShape.GetColourAsString(), I, J);

        int sameShapesCountUP = 0, sameShapesCountLEFT = 0, sameShapesCountDOWN = 0, sameShapesCountRIGHT = 0;
        // UP
        for(int i = I - 1; i >= 0; i--)
        {
            Shape checkedShape = GetShapeAt(i,J);
            if(checkedShape.GetColour() == initialShape.GetColour())
            {
                printf("\tCheck UP --- %s[%d][%d] == %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), i, J);
                sameShapesCountUP++;
            }
            else
            {
                printf("\tCheck UP --- %s[%d][%d] != %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), i, J);
                break;
            }
        }
        // LEFT
        for(int j = J - 1; j >= 0; j--)
        {
            Shape checkedShape = GetShapeAt(I,j);
            if(checkedShape.GetColour() == initialShape.GetColour())
            {
                printf("\tCheck LEFT --- %s[%d][%d] == %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), I, j);
                sameShapesCountLEFT++;
            }
            else
            {
                printf("\tCheck LEFT --- %s[%d][%d] != %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), I, j);
                break;
            }
        }
        // DOWN
        for(int i = I + 1; i <= gameBoardSize - 1; i++)
        {
            Shape checkedShape = GetShapeAt(i,J);
            if(checkedShape.GetColour() == initialShape.GetColour())
            {
                printf("\tCheck DOWN --- %s[%d][%d] == %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), i, J);
                sameShapesCountDOWN++;
            }
            else
            {
                printf("\tCheck DOWN --- %s[%d][%d] != %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), i, J);
                break;
            }
        }
        // RIGHT
        for(int j = J + 1; j <= gameBoardSize - 1; j++)
        {
            Shape checkedShape = GetShapeAt(I,j);
            if(checkedShape.GetColour() == initialShape.GetColour())
            {
                printf("\tCheck RIGHT --- %s[%d][%d] == %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), I, j);
                sameShapesCountRIGHT++;
            }
            else
            {
                printf("\tCheck RIGHT --- %s[%d][%d] != %s[%d][%d]\n",
                                                               initialShape.GetColourAsString(), I, J,
                                                               checkedShape.GetColourAsString(), I, j);
                break;
            }
        }

        int sameShapesCountVertical = sameShapesCountUP + sameShapesCountDOWN + 1;
        printf("\t\tVertical same shape count: %d\n", sameShapesCountVertical);
        int sameShapesCountHorizontal = sameShapesCountLEFT + sameShapesCountRIGHT + 1;
        printf("\t\tHorizontal same shape count: %d\n", sameShapesCountHorizontal);

        // Check if we matched at least 3 shapes in vertical axis
        if(sameShapesCountVertical >= 3)
        {
            score += sameShapesCountVertical * 10;
            RandomizeCorrectShapes(I, J, sameShapesCountUP, sameShapesCountDOWN, VERTICAL);
        }
        // Check if we matched at least 3 shapes in horizontal axis
        if(sameShapesCountHorizontal >= 3)
        {
            score += sameShapesCountHorizontal * 10;
            RandomizeCorrectShapes(I, J, sameShapesCountLEFT, sameShapesCountRIGHT, HORIZONTAL);
        }
    }

    /// @brief Randomize all matched shapes on a certain axis
    /// @param I initial shape index i
    /// @param J initial shape index j
    /// @param sameShapesCountDir1 same shape count in first direction
    /// @param sameShapesCountDir2 same shape count in second direction
    /// @param axis axis in which the shapes got matched (VERTICAL, HORIZONTAL)
    void GameLogic::RandomizeCorrectShapes(int I, int J, int sameShapesCountDir1, int sameShapesCountDir2, Axis axis)
    {
        //shapeMatrix[I][J].SetShapeColour(NONE);
        switch (axis)
        {
        case VERTICAL:
            // UP
            for(int i = 0; i <= sameShapesCountDir1; i++)
            {
                //shapeMatrix[I-i][J].SetColour(BASE);
                shapeMatrix[I-i][J].SetRandomColour();
            }
            // DOWN
            for(int i = 0; i <= sameShapesCountDir2; i++)
            {
                //shapeMatrix[I+i][J].SetColour(BASE);
                shapeMatrix[I+i][J].SetRandomColour();
            }
            
            break;
        case HORIZONTAL:
            // LEFT
            for(int j = 0; j <= sameShapesCountDir1; j++)
            {
                //shapeMatrix[I][J-j].SetColour(BASE);
                shapeMatrix[I][J-j].SetRandomColour();
            }
            // RIGHT
            for(int j = 0; j <= sameShapesCountDir2; j++)
            {
                //shapeMatrix[I][J+j].SetColour(BASE);
                shapeMatrix[I][J+j].SetRandomColour();
            }
            break;
        default:
            break;
        }
    }

    /// @brief Move cursor or shift shapes
    /// @param direction movement direction (UP, LEFT, DOWN, RIGHT)
    void GameLogic::Move(Direction direction)
    {
        ShapeColour currentShapeColour = currentShape.GetColour();
        switch (direction)
        {
        case UP:
            if (currentI > 0)
            {
                // Check if a shift needs to be done instead
                if (currentShape.GetStatus() == SELECTED)
                {
                    currentShape.SetColour(shapeUp.GetColour());
                    shapeUp.SetColour(currentShapeColour);
                    printf("Shifted UP --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                       shapeUp.GetColourAsString(), currentI, currentJ,
                                                                       currentShape.GetColourAsString(), currentI - 1, currentJ);
                    CheckShift(currentShape, shapeUp, UP);
                }
                else    // just move cursor
                {
                    isSomethingSelected = false;
                    printf("Moved UP --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                     currentShape.GetColourAsString(), currentI, currentJ,
                                                                     shapeUp.GetColourAsString(), currentI - 1, currentJ);
                }
                currentShape.SetStatus(NONE);
                currentI--;
            }
            else    // reached upper limit
            {
                printf("Couldn't move UP anymore!\n");
            }
            break;
        case LEFT:
            if (currentJ > 0)
            {
                // Check if a shift needs to be done instead
                if (currentShape.GetStatus() == SELECTED)
                {
                    currentShape.SetColour(shapeLeft.GetColour());
                    shapeLeft.SetColour(currentShapeColour);
                    printf("Shifted LEFT --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                         shapeLeft.GetColourAsString(), currentI, currentJ,
                                                                         currentShape.GetColourAsString(), currentI, currentJ - 1);
                    CheckShift(currentShape, shapeLeft, LEFT);
                }
                else    // just move cursor
                {
                    isSomethingSelected = false;
                    printf("Moved LEFT --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                       currentShape.GetColourAsString(), currentI, currentJ,
                                                                       shapeLeft.GetColourAsString(), currentI, currentJ - 1);
                }
                currentShape.SetStatus(NONE);
                currentJ--;
            }
            else    // reached left limit
            {
                printf("Couldn't move LEFT anymore!\n");
            }
            break;
        case DOWN:
            if (currentI < gameBoardSize - 1)
            {
                // Check if a shift needs to be done instead
                if (currentShape.GetStatus() == SELECTED)
                {
                    currentShape.SetColour(shapeDown.GetColour());
                    shapeDown.SetColour(currentShapeColour);
                    printf("Shifted DOWN --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                         shapeDown.GetColourAsString(), currentI, currentJ,
                                                                         currentShape.GetColourAsString(), currentI + 1, currentJ);
                    CheckShift(currentShape, shapeDown, DOWN);
                }
                else    // just move cursor
                {
                    isSomethingSelected = false;
                    printf("Moved DOWN --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                       currentShape.GetColourAsString(), currentI, currentJ,
                                                                       shapeDown.GetColourAsString(), currentI + 1, currentJ);
                }
                currentShape.SetStatus(NONE);
                currentI++;
            }
            else    // reached lower limit
            {
                printf("Couldn't move DOWN anymore!\n");
            }
            break;
        case RIGHT:
            if (currentJ < gameBoardSize - 1)
            {
                // Check if a shift needs to be done instead
                if (currentShape.GetStatus() == SELECTED)
                {
                    currentShape.SetColour(shapeRight.GetColour());
                    shapeRight.SetColour(currentShapeColour);
                    printf("Shifted RIGHT --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                          shapeRight.GetColourAsString(), currentI, currentJ,
                                                                          currentShape.GetColourAsString(), currentI, currentJ + 1);
                    CheckShift(currentShape, shapeRight, RIGHT);
                }
                else    // just move cursor
                {
                    isSomethingSelected = false;
                    printf("Moved RIGHT --- %s[%d][%d] -> %s[%d][%d]\n",
                                                                        currentShape.GetColourAsString(), currentI, currentJ,
                                                                        shapeRight.GetColourAsString(), currentI, currentJ + 1);
                }
                currentShape.SetStatus(NONE);
                currentJ++;
            }
            else    // reached right limit
            {
                printf("Couldn't move RIGHT anymore!\n");
            }
            break;
        default:
            printf("Incorrect movement!");
            break;
        }
        currentShape.SetStatus(SELECTABLE);
    }

    /// @brief Set current shape status as SELECTED (SELECTABLE if already SELECTED)
    void GameLogic::SelectShape()
    {
        if (currentShape.GetStatus() != SELECTED)
        {
            currentShape.SetStatus(SELECTED);
            isSomethingSelected = true;
            printf("Selected %s[%d][%d]\n", currentShape.GetColourAsString(), currentI, currentJ);
        }
        else
        {
            currentShape.SetStatus(SELECTABLE);
            isSomethingSelected = false;
            printf("Deselected %s[%d][%d]\n", currentShape.GetColourAsString(), currentI, currentJ);
        }
    }

}