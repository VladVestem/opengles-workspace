 #include <string>
 
 #ifndef shape
 #define shape
    namespace opengles_workspace
    {
        enum ShapeColour
        {
            BASE,
            RED,
            GREEN,
            BLUE,
            CYAN,
            MAGENTA,
            YELLOW,
            LIME,
            BEIGE,
            PINK
        };
        enum ShapeStatus
        {
            NONE,
            SELECTABLE,
            SELECTED
        };

        class Shape
        {
        private:
            ShapeColour shapeColour;
            ShapeStatus shapeStatus;

        public:
            Shape();
            ~Shape() {};

            ShapeColour GetColour();
            ShapeStatus GetStatus();
            void SetColour(ShapeColour);
            void SetRandomColour();
            void SetStatus(ShapeStatus);
            const char* GetColourAsString();
            std::string GetTexturePath();
        };
    }
 #endif