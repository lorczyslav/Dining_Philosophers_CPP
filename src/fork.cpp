#include "../include/fork.hpp"

void Fork::use()
{
    _in_use = true;
}

void Fork::leave()
{
    _in_use = false;
}