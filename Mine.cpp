#pragma once
#include "GameObjects.h"

class Mine : public StaticObject{

    public:
    Mine() : StaticObject(game_object::mine) {}

    void destroy(){
        // needed to complete, destroy Tank if Tank land on mine
        
    }



};