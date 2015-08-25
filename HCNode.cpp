#include "HCNode.hpp"

/** Simply compares the counts in two HCNodes, this and other.
 *  Uses symbol for tiebreaker.
 */
bool HCNode::operator<(const HCNode& other) {
    if (other.count != count)
        return (other.count < count);
    return (other.symbol > symbol);
}

bool comp(HCNode* one, HCNode* other) {
    return (other < one);
}
