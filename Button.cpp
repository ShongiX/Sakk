//
// Created by laccs on 4/7/2022.
//

#include "Button.hpp"

#include <utility>
#include <iostream>

using namespace genv;

class Menu;

Button::Button(Menu* m,int x, int y, int sx, int sy, std::string name, std::function<void()> func) : Widget(m,x,y,sx,sy) {
    _name = std::move(name);
    _func = std::move(func);
}

void Button::handle(const genv::event& ev) {

    if (ev.button == btn_left) {
        pressed = true;
    } else if (ev.button == -btn_left) {
        pressed = false;
    }

    if (hover(ev) && _state == NEUTRAL) _state = HOVER;
    if (!hover(ev) && _state == HOVER) _state = NEUTRAL;
    if (hover(ev) && pressed) _state = CLICKED;
    if (_state == CLICKED && !pressed) {
        if (hover(ev)) {
            _func();
        }
        _state = NEUTRAL;
    }

    //std::cout << "State: " << (int)_state << std::endl;
}

void Button::draw() {
    Color bc; //Box Color
    Color oc; //Outline Color

    if (_state == NEUTRAL) {
        bc = Color(100,100,100);
        oc = Color(255,255,255);
    } else if (_state == HOVER) {
        bc = Color(150,150,150);
        oc = Color(255,255,255);
    } else {
        bc = Color(150,150,150);
        oc = Color(0,0,0);
    }

    /*if (_focus) {
        if (pressed) {
            bc = Color(150,150,150);
            oc = Color(0,0,0);
        } else {
            bc = Color(150,150,150);
            oc = Color(255,255,255);
        }
    } else {
        bc = Color(100,100,100);
        oc = Color(255,255,255);
    }*/

    //Box
    gout << color(bc.r,bc.g,bc.b) << move_to(_x,_y) << box(_sx,_sy);
    //Shadow
    gout << color(0,0,0) << move_to(_x+_sx/2- gout.twidth(_name)/2,_y+_sy/2-( gout.cascent()+ gout.cdescent())/2) << move(2,2) << genv::text(_name);
    //Text
    gout << color(255,255,255) << move_to(_x+_sx/2- gout.twidth(_name)/2,_y+_sy/2-( gout.cascent()+ gout.cdescent())/2) << genv::text(_name);
    //Outline
    gout << color(oc.r,oc.g,oc.b) << move_to(_x,_y) << line(_sx,0) << move_to(_x,_y) << line(0,_sy);
    gout << color(255-oc.r,255-oc.g,255-oc.b) << move_to(_x+_sx,_y+_sy) << line(-_sx,0) << move_to(_x+_sx,_y+_sy) << line(0,-_sy);
}

bool Button::isFocusable() {
    return true;
}

bool Button::isFocus(const genv::event &ev) const {
    return ev.type == genv::ev_mouse && ev.pos_x > _x && ev.pos_x < _x + _sx && ev.pos_y > _y && ev.pos_y < _y + _sy;
}

bool Button::hover(const genv::event &ev) const {
    int ex = ev.pos_x;
    int ey = ev.pos_y;

    return ex >= _x && ey >= _y && ex <= _x + _sx && ey <= _y + _sy;
}

