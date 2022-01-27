// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

// Represents a single coordinate.
struct Location {
  bool discovered = false;
    int row = 0;
    int col = 0;
    int mapsize = 0;
    char format;
    char symbol;
    char first_char;
    Location *prev;
};
