#include "core/joypad.h"

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

uint8_t joypadDebounce(uint8_t *jMem)
{

    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (ch == 'w')
    {
        return J_UP;
    }
    else if (ch == 'a')
    {
        return J_LEFT;
    }
    else if (ch == 's')
    {
        return J_RIGHT;
    }
    else if (ch == 'd')
    {
        return J_DOWN;
    }
    else if (ch == 'k')
    {
        return J_A;
    }
    else if (ch == 'j')
    {
        return J_B;
    }
    else if (ch == ',')
    {
        return J_SELECT;
    }
    else if (ch == '.')
    {
        return J_START;
    }
    return 0;
}
