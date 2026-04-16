#include "Code.h"

Code::Code() {
    this->top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i++)
        this->bits[i] = 0;
}

bool Code::empty() const {
    return this->top == 0;
}

bool Code::full() const {
    return this->top == MAX_CODE_SIZE;
}

uint32_t Code::size() const {
    return this->top; //since code is a stack, top is the first empty position
}

bool Code::push_bit(uint8_t bit) {
    if (this->full())
        return false;
    //add the bit at the current top and increase the top
    this->bits[this->top] = bit;
    this->top++;
    return true;
}

bool Code::pop_bit(uint8_t &bit) {
    if (this->empty())
        return false;
    //go to the last element and store it in the bit
    this->top--;
    bit = this->bits[this->top];
    return true;
}

uint8_t Code::get_bit(uint32_t index) const {
    if (index < this->top)
        return this->bits[index];
    return 0;
}
