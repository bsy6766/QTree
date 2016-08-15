#include "QTree.h"
QTree::QTree(const cocos2d::Rect& boundary, int level) : nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr), boundary(boundary), leaf(true), level(level)
{}

QTree::~QTree()
{
	clear();
}

void QTree::clear()
{
	if (nw != nullptr)
	{
		delete nw;
	}

	if (ne != nullptr)
	{
		delete ne;
	}

	if (sw != nullptr)
	{
		delete sw;
	}

	if (se != nullptr)
	{
		delete se;
	}

	this->boxes.clear();
}

void QTree::subDivide()
{
	//cocos2d::log("Subdividing on level %d", this->level);
	//sub divide only happen when there is more than 1 object.
	this->leaf = false;

	float width = this->boundary.size.width / 2.0f;
	float height = this->boundary.size.height / 2.0f;

	//top left 
	nw = new QTree(cocos2d::Rect(this->boundary.getMinX(), this->boundary.getMidY(), width, height), this->level + 1);

	//top right
	ne = new QTree(cocos2d::Rect(this->boundary.getMidX(), this->boundary.getMidY(), width, height), this->level + 1);

	//bot left
	sw = new QTree(cocos2d::Rect(this->boundary.getMinX(), this->boundary.getMinY(), width, height), this->level + 1);

	//bot right
	se = new QTree(cocos2d::Rect(this->boundary.getMidX(), this->boundary.getMinY(), width, height), this->level + 1);

	//so...replace the object
	auto entity = this->boxes.begin();
	std::list<cocos2d::Rect> remains;

	for (auto entity : this->boxes)
	{
		remains.push_back(entity);
	}

	this->boxes.clear();

	for (auto box : remains)
	{
		if (this->nw->insert(box))
		{
			continue;
		}
		else if (this->ne->insert(box))
		{
			continue;
		}
		else if (this->sw->insert(box))
		{
			continue;
		}
		else if (this->sw->insert(box))
		{
			continue;
		}
		else
		{
			this->boxes.push_back(box);
		}
	}

	remains.clear();
}

bool QTree::insert(const cocos2d::Rect& boundingBox)
{
	//Zombie and bullet will be always in the world boundary no matter where we see
	if (this->boundary.intersectsRect(boundingBox))
	{
		//check if it has subs
		if (leaf)
		{
			//don't have sub. check max level
			if (this->level < MAX_LEVEL)
			{
				//not max level yet. subdivide. objects existing this node should be repositioned to new subs
				if (this->boxes.size() < CAPACITY)
				{
					this->boxes.push_back(boundingBox);
					return true;
				}
				else
				{
					subDivide();

					//insert new object to subs
					if (this->nw->insert(boundingBox))
					{
						return true;
					}
					else if (this->ne->insert(boundingBox))
					{
						return true;
					}
					else if (this->sw->insert(boundingBox))
					{
						return true;
					}
					else if (this->sw->insert(boundingBox))
					{
						return true;
					}
					else
					{
						//doesn't fit. remain here
						this->boxes.push_back(boundingBox);
						return true;
					}
				}

			}
			else
			{
				this->boxes.push_back(boundingBox);
				return true;
			}
		}
		else
		{					
			//insert new object to subs
			if (this->nw->insert(boundingBox))
			{
				return true;
			}
			else if (this->ne->insert(boundingBox))
			{
				return true;
			}
			else if (this->sw->insert(boundingBox))
			{
				return true;
			}
			else if (this->sw->insert(boundingBox))
			{
				return true;
			}
			else
			{
				//doesn't fit. remain here
				this->boxes.push_back(boundingBox);
				return true;
			}
		}
	}
	else
	{
		return false;
	}

	return false;
}

void QTree::queryAll(const cocos2d::Rect& queryingArea, std::list<cocos2d::Rect>& nearBoundingBoxes)
{
	if (this->boundary.intersectsRect(queryingArea))
	{
		if (leaf)
		{
			for (auto entity : this->boxes)
			{
				nearBoundingBoxes.push_back(entity);
			}
		}
		else
		{
			//not a leaf. Add objects in this node and proceed to children
			for (auto box : this->boxes)
			{
				nearBoundingBoxes.push_back(box);
			}

			if (this->nw != nullptr)
			{
				this->nw->queryAll(queryingArea, nearBoundingBoxes);
			}

			if (this->ne != nullptr)
			{
				this->ne->queryAll(queryingArea, nearBoundingBoxes);
			}

			if (this->sw != nullptr)
			{
				this->sw->queryAll(queryingArea, nearBoundingBoxes);
			}

			if (this->se != nullptr)
			{
				this->se->queryAll(queryingArea, nearBoundingBoxes);
			}
		}
	}
}

bool QTree::containsRect(const cocos2d::Rect& objRect)
{
	if (objRect.getMaxX() <= this->boundary.getMaxX() &&
		objRect.getMinX() >= this->boundary.getMinX() &&
		objRect.getMaxY() <= this->boundary.getMaxY() &&
		objRect.getMinY() >= this->boundary.getMinY())
	{
		return true;
	}
	else
	{
		return false;
	}
}