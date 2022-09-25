
#define REFRESH_0() move(0,0); refresh();
#define CLR_LINE(y) move(y,0); clrtoeol();

#define GRID_X 5            // Horizontal margin for the cell grid
#define GRID_Y 3
#define GRID_C 30           // Max number of items. Items per page should change depending on the terminal size
int GRID_CPP = GRID_C;      // Cells displayed per page. Will be calculated in draw_grid()
#define MIN_GRID_CW 1       // Min width of the cells 
int GRID_CW = MIN_GRID_CW;  // Default width of cells. Will change if we try to display large numbers

void draw_grid() {
    // Define in case we want to pass this as args in the future
    const int x      = GRID_X;
    const int y      = GRID_Y;
    const int cell_w = GRID_CW;
    int term_w       = getmaxx(stdscr);     // Get terminal width

    // (terminal width - margin * 2) / (cell width + cell border)
    const int cells = (term_w - GRID_X * 2) / (cell_w + 1);
    if (cells < 1) return;
    GRID_CPP = cells;

    // TODO: DELME
    mvprintw(10, 10, "W: %d | Cells: %d", term_w, cells);

    // First clear the cell lines to remove residual chars from resizes, etc.
    CLR_LINE(y);
    CLR_LINE(y+1);
    CLR_LINE(y+2);

    // Initial left border
    mvprintw(y,   x, "+");
    mvprintw(y+1, x, "|");
    mvprintw(y+2, x, "+");

    // TODO: Add 'next page' options (n = cells * cur page number)
    for (int n = 0; n < cells; n++) {
        /* Center of each cell. We use +1 because of the initial left border and 2*n because
         * its the space of a cell times the current cell number:
         *   -+-+-+
         *    | | |
         *   -+-+-+
         *   ^^
         *
         * We iterate the cell w.
         */
        for (int i = 0; i < cell_w; i++) {
            // (base x pos + left border) + ((cell width + right border) * cell number) + inner cell position
            mvprintw(y,   (x+1) + ((cell_w+1)*n) + i, "-");
            mvprintw(y+1, (x+1) + ((cell_w+1)*n) + i, " ");
            mvprintw(y+2, (x+1) + ((cell_w+1)*n) + i, "-");
        }

        // Right border
        // base x pos + cell inner width + right border * current cell number
        mvprintw(y,   x+(cell_w+1)*n, "+");
        mvprintw(y+1, x+(cell_w+1)*n, "|");
        mvprintw(y+2, x+(cell_w+1)*n, "+");
    }

    // Rightmost border
    mvprintw(y,   x+(cell_w+1)*cells, "+");
    mvprintw(y+1, x+(cell_w+1)*cells, "|");
    mvprintw(y+2, x+(cell_w+1)*cells, "+");

    REFRESH_0();
}


void fill_cell(int idx, const char* str) {
    const int gx   = GRID_X;
    const int gy   = GRID_Y;
    const int gcpp = GRID_CPP;
    const int gcw  = GRID_CW;

    // Return if we are out of bounds
    if (idx >= gcpp) return;

    // Center of y, base x pos + leftmost border + (right border of the cell + cell width) * cell number
    mvprintw(gy+1, gx+1 + (1+gcw) * idx, "%s", str);

    REFRESH_0();
}
