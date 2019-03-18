#include "blockIterator.h"


blockIterator::blockIterator(std::vector<deltaUnit> &units, posn tl, posn br, unsigned int width):
    units{units}, pos{tl}, tl{tl}, br{br}, width{width}, unitLength{units[0].size()} {
        innerUnitPos = 0;
    }

blockIterator &blockIterator::operator++() {
    if (innerUnitPos < unitLength-1) {
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

bool blockIterator::end() {
    return innerUnitPos == unitLength-1 && pos.x == br.x && pos.y == br.y;
}

bool blockIterator::operator!=(blockIterator other) {
    return pos != other.pos;
}

Color &blockIterator::operator*() {
    try {
        return units[pos.y*width + pos.x].at(innerUnitPos);
    }
    catch (std::logic_error e) {
        std::cout << e.what() << std::endl;
        std::cout << pos.y << " " << pos.x << " " << units.size() << std::endl;
        throw std::logic_error("iterator out of range");
    }
}

void blockIterator::reset() {
    pos = tl;
}

