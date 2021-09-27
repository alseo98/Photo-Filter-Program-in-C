#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <cs50.h>

// Convert image to grayscale

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            
            double h = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            int average = round(h);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++)
    {

        for(int j = 0; j < (width / 2); j++)
        {
            RGBTRIPLE *temp = malloc(3);
            *temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = *temp;
            free(temp);
        }
    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalBlue = 0;
            int totalRed = 0;
            int totalGreen = 0;
            float count = 0.0;
            
            for (int a = -1; a <= 1; a++)
            {
                for (int b = -1; b <= 1; b++)
                {
                    if (((i + a) >= 0 && (i + a) < height) && ((j + b) >= 0 && (j + b) < width))
                    {
                        totalBlue += image[i + a][j + b].rgbtBlue;
                        totalRed += image[i + a][j + b].rgbtRed;
                        totalGreen += image[i + a][j + b].rgbtGreen;
                        count++;
                    }
                }
            }
            
            temp[i][j].rgbtBlue = round(totalBlue / (float)count);
            temp[i][j].rgbtRed = round(totalRed / (float)count);
            temp[i][j].rgbtGreen = round(totalGreen / (float)count);
        }
    }
     for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int cap = 255;
    int FinalRed = 0; 
    int FinalGreen = 0; 
    int FinalBlue = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    
    double factorGx[3][3] = {{-1.0, 0.0, 1.0}, {-2.0, 0.0, 2.0}, {-1.0, 0.0, 1.0}};
    double factorGy[3][3] = {{-1.0, -2.0, -1.0}, {0.0, 0.0, 0.0}, {1.0, 2.0, 1.0}};
    
    

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE AImatrix[3][3];
            long GxRed = 0;
            long GxGreen = 0;
            long GxBlue = 0;
            
            long GyRed = 0;
            long GyGreen = 0;
            long GyBlue = 0;
            
            
        for (int a = -1; a <= 1; a++)
        {
            for (int b = -1; b <= 1; b++)
            {
                if (((i + a) >= 0 && (i + a) < height) && ((j + b) >= 0 && (j + b) < width))
                    AImatrix[a + 1][b + 1] = image[i + a][j + b];
                else
                {
                AImatrix[a + 1][b + 1].rgbtRed = 0;
                AImatrix[a + 1][b + 1].rgbtGreen = 0;
                AImatrix[a + 1][b + 1].rgbtBlue = 0;
                }
            }
        }
        
        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                GxRed += AImatrix[x][y].rgbtRed * factorGx[x][y];
                GxGreen += AImatrix[x][y].rgbtGreen * factorGx[x][y];
                GxBlue += AImatrix[x][y].rgbtBlue * factorGx[x][y];
                
                GyRed += AImatrix[x][y].rgbtRed * factorGy[x][y];
                GyGreen += AImatrix[x][y].rgbtGreen * factorGy[x][y];
                GyBlue += AImatrix[x][y].rgbtBlue * factorGy[x][y];
                
            }
        }
        //SOBEL EQUATION
        FinalRed = round(sqrtf(GxRed * GxRed + GyRed * GyRed));
        FinalGreen = round(sqrtf(GxGreen * GxGreen + GyGreen * GyGreen));    
        FinalBlue = round(sqrtf(GxBlue * GxBlue + GyBlue * GyBlue));    
        
        if (FinalRed > cap)
        temp[i][j].rgbtRed = cap;
        else
        temp[i][j].rgbtRed = FinalRed;
        
        if (FinalGreen > cap)
        temp[i][j].rgbtGreen = cap;
        else
        temp[i][j].rgbtGreen = FinalGreen;
        
       if (FinalBlue > cap)
        temp[i][j].rgbtBlue = cap;
        else
        temp[i][j].rgbtBlue = FinalBlue;
        }
    }
    
     for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
             image[i][j] = temp[i][j];
        }
    }
    
    return;
}
