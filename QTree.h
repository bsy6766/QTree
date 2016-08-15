#ifndef QTREE_H
#define QTREE_H

#include "cocos2d.h"
#include <list>

//The capacity of leaf tree can have before it sub divides
#define CAPACITY 3
//The maximum level of tree it can sub divide.
#define MAX_LEVEL 6

/**
*	@class QTree
*	@brief Simple implementation of QuadTree with cocos2d-x. 
*	@note This quad tree manages cocos2d-x bounding boxes instaead of entity.
*/
class QTree
{
private:
	//4 divided areas
	QTree* nw;	//North West
	QTree* ne;	//North East
	QTree* sw;	//South West
	QTree* se;	//South East

	//The tree's level. Starts from 0
	int level;

	//True if this tree is leaf, which means has no sub division
	bool leaf;

	//The boundary of this quad tree.
	cocos2d::Rect boundary;

	//Boxes
	std::list<cocos2d::Rect> boxes;

	//sub divide current quad tree
	void subDivide();

	/**
	*	@brief Checks if passed bounding box is inside of boundary.
	*
	*	@retval true Bounding box is completely in the boundary
	*	@retval false Bounding box is not in the boundary. But can be intersect.
	*/
	bool containsRect(const cocos2d::Rect& objRect);
public:
	//Contructor
	QTree(const cocos2d::Rect& boundary, int level);

	//Destructor. Calls clear()
	~QTree();

	//Insert bounding box to tree
	bool insert(const cocos2d::Rect& boundingBox);

	//Clears current tree. Deletes all sub division and bounding boxes.
	void clear();

	/**
	*	@brief Query all bounding boxes that intersects querying area.
	*
	*	@param queryingArea An area to query.
	*	@param nearBoundingBoxes Container to retreive queried bounding boxes
	*/
	void queryAll(const cocos2d::Rect& queryingArea, std::list<cocos2d::Rect>& nearBoundingBoxes);
};

#endif