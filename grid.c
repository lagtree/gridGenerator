#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define Functions
int is_safe(int **grid, int height, int width, int x, int y, int color);
int fill_grid(int **grid, int height, int width, int num_colors);
void random_fill(int **grid, int height, int width, int num_colors);
void shuffle_colors(int *colors, int num_colors);

int main() {
    int height, width, num_colors;

    // Getting user input
    printf("Enter the height of the grid: ");
    scanf("%d", &height);
    printf("Enter the width of the grid: ");
    scanf("%d", &width);
    printf("Enter the number of colors: ");
    scanf("%d", &num_colors);

    // Create and initialize grid
    int **grid = (int **)malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        grid[i] = (int *)malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) {
            grid[i][j] = -1;  // Initialize with an invalid color
        }
    }

    // Try to fill the grid with valid color placement
    if (!fill_grid(grid, height, width, num_colors)) {
        printf("Unable to fill the grid without adjacent same colors. Randomizing...\n");
        random_fill(grid, height, width, num_colors);
    }

    // Output the grid
    printf("Generated grid:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}

// Function to check if it's safe to place a color at a given position
int is_safe(int **grid, int height, int width, int x, int y, int color) {
    // Check above
    if (x > 0 && grid[x-1][y] == color) return 0;
    // Check below
    if (x < height-1 && grid[x+1][y] == color) return 0;
    // Check left
    if (y > 0 && grid[x][y-1] == color) return 0;
    // Check right
    if (y < width-1 && grid[x][y+1] == color) return 0;

    return 1;
}

// Recursive backtracking function to fill the grid
int fill_grid(int **grid, int height, int width, int num_colors) {
    int colors[num_colors];
    
    // Initialize colors array
    for (int i = 0; i < num_colors; i++) {
        colors[i] = i;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Try every color in a shuffled order
            shuffle_colors(colors, num_colors);
            int success = 0;
            for (int k = 0; k < num_colors; k++) {
                int color = colors[k];
                if (is_safe(grid, height, width, i, j, color)) {
                    grid[i][j] = color;
                    success = 1;
                    break;
                }
            }

            // If we fail to assign any color, return failure
            if (!success) return 0;
        }
    }
    return 1;  // Successfully filled the grid
}

// Function to randomly fill the grid if no valid grid can be generated
void random_fill(int **grid, int height, int width, int num_colors) {
    int color_counts[num_colors];
    int total_cells = height * width;

    // Initialize color counts
    for (int i = 0; i < num_colors; i++) {
        color_counts[i] = total_cells / num_colors;
    }

    // Distribute remaining cells (if any)
    for (int i = 0; i < total_cells % num_colors; i++) {
        color_counts[i]++;
    }

    // Shuffle and fill grid
    srand(time(0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int color;
            do {
                color = rand() % num_colors;
            } while (color_counts[color] == 0);
            grid[i][j] = color;
            color_counts[color]--;
        }
    }
}

// Function to shuffle the colors array
void shuffle_colors(int *colors, int num_colors) {
    for (int i = 0; i < num_colors; i++) {
        int j = rand() % num_colors;
        int temp = colors[i];
        colors[i] = colors[j];
        colors[j] = temp;
    }
}
