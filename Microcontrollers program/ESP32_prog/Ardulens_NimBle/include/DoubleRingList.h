#pragma once

#include <Arduino.h>

template <typename T>
class DoubleCycleList
{
private:
	class Element
	{
	public:
		Element *previous_;
		Element *next_;
		T *value_;
		Element()
		{
		}

		Element(T *value, Element *next, Element *previous)
		{
			value_ = value;
			next_ = next;
			previous_ = previous;
		}
		~Element()
		{
		}
	};

	Element *head_ = nullptr;
	uint16_t lenght_ = 0;

	uint16_t cursorItemIndex_ = 0;
	Element *cursorItem_ = nullptr;

	Element *getElement(int index)
	{
		Element *needElem = head_;
		for (uint16_t i = 0; i < index; i++)
		{
			needElem = needElem->next_;
		}
		return needElem;
	}

public:
	DoubleCycleList()
	{
	}

	void add(T *value)
	{
		Element *newElem = (Element *)malloc(sizeof(Element));
		newElem->value_ = value;
		if (head_ == nullptr)
		{
			head_ = newElem;
			head_->next_ = head_->previous_ = newElem;
		}
		else
		{
			head_->previous_->next_ = newElem;
			newElem->previous_ = head_->previous_;
			newElem->next_ = head_;
			head_->previous_ = newElem;
		}
		lenght_++;
	}

	T *get(int index)
	{
		if (lenght_ == 0)
			return nullptr;
		return getElement(index)->value_;
	}

	void remove(int index, bool deleteValue = false)
	{
		if (lenght_ == 0)
			return;
		Element *needElem = head_;
		for (int i = 0; i < index; i++)
		{
			needElem = needElem->next_;
		}
		if (lenght_ == 1)
		{
			if (deleteValue)
				delete needElem->value_;
			free(needElem);
			head_ = nullptr;
		}
		else
		{
			if (index == 0)
				head_ = needElem->next_;
			needElem->previous_->next_ = needElem->next_;
			needElem->next_->previous_ = needElem->previous_;
			if (deleteValue)
				delete needElem->value_;
			free(needElem);
		}
		lenght_--;
	}

	uint16_t getLenght()
	{
		return lenght_;
	}

	void setCursor(int16_t index)
	{
		if (index > lenght_)
		{
			cursorItemIndex_ = index - lenght_;
		}
		else if (index < 0)
		{
			if (index * -1 > lenght_)
			{
				index = index + lenght_;
			}
			cursorItemIndex_ = lenght_ + index;
		}
		else
		{
			cursorItemIndex_ = index;
		}
		cursorItem_ = getElement(cursorItemIndex_);
	}

	T *getCursorItem()
	{
		return cursorItem_->value_;
	}
	T *moveCursorNext()
	{
		if (cursorItem_ == nullptr)
			setCursor(cursorItemIndex_);
		cursorItemIndex_ = cursorItemIndex_ + 1 >= lenght_ ? 0 : cursorItemIndex_ + 1;

		cursorItem_ = cursorItem_->next_;
		return cursorItem_->value_;
	}

	T *moveCursorPrevious()
	{
		if (cursorItem_ == nullptr)
			setCursor(cursorItemIndex_);
		cursorItemIndex_ = cursorItemIndex_ == 0 ? lenght_ - 1 : cursorItemIndex_ - 1;
		cursorItem_ = cursorItem_->previous_;
		return cursorItem_->value_;
	}

	void deleteCursorItem()
	{
		delete cursorItem_->value_;
	}

	int getCursorItemIndex()
	{
		return cursorItemIndex_;
	}

	void swapElements(int index1, int index2)
	{
		Element *el1 = getElement(index1);
		Element *el2 = getElement(index2);
		T *temp = el1->value_;
		el1->value_ = el2->value_;
		el2->value_ = temp;
	}

	T *getBy(std::function<bool(T *data)> func, uint16_t* index = nullptr)
	{
		Element *needElem = head_;
		for (uint16_t i = 0; i < lenght_; i++)
		{
			if (func(needElem->value_))
			{
				if(index != nullptr)
					*index = i;
				return needElem->value_;
			}
			needElem = needElem->next_;
		}
		return nullptr;
	}

	void setCursorByObject(T *data)
	{
		Element *elem = head_;
		for (uint16_t i = 0; i < lenght_; i++)
		{
			if (elem->value_ == data)
			{
				cursorItemIndex_ = i;
				cursorItem_ = elem;
				return;
			}
			elem = elem->next_;
		}
	}
};