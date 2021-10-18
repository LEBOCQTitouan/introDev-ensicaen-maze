#include "maze.h"

int getEntityPenaltyValue(entityType type) {
    switch (type)
    {
    case TREASURE:  return 100;
    case TRAP:      return -100;
    case TROLL:     return -100;
    case GHOST:     return -200;
    default:        return 0;
    }
}

void defaultAction(int xPlayer, int yPlayer, entity * en, maze * m) { return; } // default action do nothing

void ghostAction(int xPlayer, int yPlayer, entity * en, maze * m) {
    en->x = rand() % m->width;
    en->y = rand() % m->height;
}

void trollAction(int xPlayer, int yPlayer, entity * en, maze * m) {
    switch (rand() % 5)
    {
    case MOVE_UP:
        en->y--;
        break;
    case MOVE_DOWN:
        en->y++;
        break;
    case MOVE_LEFT:
        en->x--;
        break;
    case MOVE_RIGHT:
        en->x++;
        break;
    
    default:
        break;
    }
}

void keyAction(int xPlayer, int yPlayer, entity * en, maze * m) {
    if (((xPlayer == (*en).x) && (yPlayer == (*en).y)) && !m->isUnlocked) {
        m->isUnlocked = true;
    }
}

mobility_action getMobilityAction(entityType type) {
    switch (type)
    {
    case GHOST:     return &ghostAction;
    case TROLL:     return &trollAction;
    case KEY:       return &keyAction;
    default:        return &defaultAction;
    }
}