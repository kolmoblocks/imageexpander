#include "blockIterator.h"
    blockIterator::blockIterator(std::vector<deltaUnit> &units, posn tl, posn br, int width): 
        units{units}, pos{tl}, tl{tl}, br{br}, width{width}, unitLength{units[0].size()} {}

    blockIterator &operator++() {
        if (innerUnitPos < unitLength) {
            ++innerUnitPos;
        }
        else {
            innerUnitPos = 0;
            if (pos.x == br.x) {
                pos.x = tl.x;
                ++pos.y;
            }
            else if (pos.y > br.y || pos.x > br.x) {
                throw std::logic_error("block iterator position out of range");
            }
            else {
                ++pos.x;
            }
        }
        return *this;
    }

    bool end() {
        return innerUnitPos == unitLength-1 && pos.x == br.x && pos.y == br.y;
    }

    bool operator!=(blockIterator other) {
        return pos != other.pos;
    }

    color &operator*() {
        return units[pos.y*width + pos.x].at(innerUnitPos);
    }

    void reset() {
        pos = tl;
    }

