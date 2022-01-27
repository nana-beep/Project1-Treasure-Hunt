//  Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

// Represents a single coordinate.
struct location {
    bool discovered = false;
    char previos_step;
    char terrian = '.';
};


struct coordinate {
    unsigned int row, col;
};

