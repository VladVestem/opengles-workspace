#include <shape.hpp>
#include <random>

namespace opengles_workspace
{
    std::random_device rd;                          // obtain a random number from hardware
    std::mt19937 gen(rd());                         // seed the generator
    std::uniform_int_distribution<> distr(1, 3);    // define the range

    Shape::Shape()
    {
        shapeColour = ShapeColour(distr(gen));
        shapeStatus = NONE;
    }

    /// @brief Set colour to Shape
    /// @param colour colour you want to set (RED, GREEN, BLUE)
    void Shape::SetColour(ShapeColour colour)
    {
        this->shapeColour = colour;
    }

    /// @brief Set random colour to Shape (RED, GREEN, BLUE)
    void Shape::SetRandomColour()
    {
        this->shapeColour = ShapeColour(distr(gen));
    }

    /// @brief Set status to Shape
    /// @param status status you want to set (BASE, SELECTABLE, SELECTED)
    void Shape::SetStatus(ShapeStatus status)
    {
        this->shapeStatus = status;
    }

    /// @brief Get colour of Shape
    /// @return (RED, GREEN, BLUE)
    ShapeColour Shape::GetColour()
    {
        return this->shapeColour;
    }

    /// @brief Get status of Shape
    /// @return (BASE, SELECTABLE, SELECTED)
    ShapeStatus Shape::GetStatus()
    {
        return this->shapeStatus;
    }

    /// @brief Get colour of Shape as string
    /// @return ("RED", "GREEN", "BLUE")
    const char* Shape::GetColourAsString()
    {
        const char* returnedString;
        switch (this->shapeColour)
        {
        case RED:
            returnedString = "RED";
            break;
        case GREEN:
            returnedString = "GREEN";
            break;
        case BLUE:
            returnedString = "BLUE";
            break;
        default:
            returnedString = "empty";
            break;
        }
        return returnedString;
    }

    /// @brief Get texture path of Shape as string
    /// @return Texture path
    std::string Shape::GetTexturePath()
    {
        std::string returnedString = "";
        switch (this->shapeColour)
        {
        case RED:
            returnedString = "../images/red";
            break;
        case GREEN:
            returnedString = "../images/green";
            break;
        case BLUE:
            returnedString = "../images/blue";
            break;
        default:
            returnedString = "../images/base";
            break;
        }

        switch (this->shapeStatus)
        {
        case SELECTABLE:
            returnedString = returnedString + "_selectable";
            break;
        case SELECTED:
            returnedString = returnedString + "_selected";
            break;
        default:
            break;
        }

        return returnedString + ".jpg";
    }
}