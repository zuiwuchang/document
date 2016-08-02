//driver utility.hpp
#pragma once

#pragma code_seg()
template<typename T>
class LinkList
{

private:
	typedef struct _NOTE_
	{
		T data;
	private:
		_NOTE_* pre;
		_NOTE_* next;
	friend class LinkList;

	}NOTE,*PNOTE;
public:
	typedef PNOTE POS;
private:
	POS link_first;
	POS link_end;

	ULONG total_element; 

	POOL_TYPE pool_type;
public:
	LinkList()
	{
		init();
	}
	~LinkList()
	{
		clear();
	}
	void init()
	{
		pool_type=NonPagedPool;
		link_first=link_end=0;
		total_element=0;
	}
	void clear()
	{
		while(link_first)
		{
			link_end=link_first;
			link_first=link_first->next;
			ExFreePool(link_end);
		}
		link_first=link_end=0;
		total_element=0;
	}

	void setPoolType(POOL_TYPE pool_type)
	{
		this->pool_type=pool_type;
	}

	bool push_back(const T& data)
	{
		PNOTE lp=(PNOTE)ExAllocatePool(pool_type,sizeof(NOTE));
		if(!lp)
		{
			return false;
		}

		lp->data=data;
		lp->next=0;

		if(link_end)
		{
			lp->pre=link_end;
			link_end->next=lp;
			link_end=lp;
		}
		else
		{
			link_first=link_end=lp;
			lp->pre=0;
		}
		++total_element;
		return true;
	}	
	bool push_front(const T& data)
	{
		
		PNOTE lp=(PNOTE)ExAllocatePool(pool_type,sizeof(NOTE));
		if(!lp)
		{
			return false;
		}

		lp->data=data;
		lp->pre=0;

		if(link_first)
		{
			lp->next=link_first;
			link_first->pre=lp;
			link_first=lp;
		}
		else
		{
			link_first=link_end=lp;
			lp->next=0;
		}
		++total_element;
		return true;
	}
	bool insert(POS pos,const T& data)
	{
		PNOTE lp=new NOTE;
		PNOTE lp=(PNOTE)ExAllocatePool(pool_type,sizeof(NOTE));
		if(lp)
		{
			lp->next=pos;
			lp->pre=pos->pre;
			pos->pre->next=lp;
			lp->data=data;
			return true;
		}
		total_element++;
		return false;
	}
	
	void pop_back()
	{
		if(link_end)
		{
			if(link_end==link_first)
			{
				ExFreePool(link_end);
				link_end=link_first=0;
			}
			else
			{
				PNOTE tmp=link_end;
				link_end=tmp->pre;
				link_end->next=0;
				ExFreePool(tmp);
			}
			--total_element;
		}
	}
	void pop_front()
	{
		if(link_first)
		{
			if(link_end==link_first)
			{
				ExFreePool(link_end);
				link_end=link_first=0;
			}
			else
			{
				PNOTE tmp=link_first;
				link_first=tmp->next;
				link_first->pre=0;
				ExFreePool(tmp);
			}
			--total_element;
		}
	}

	void erase(const POS pos)
	{
		if(link_first==link_end && link_first==pos)
		{
			total_element=0;
			link_first=link_end=0;
		}
		else if(link_first==pos)
		{
			link_first=link_first->next;
			link_first->pre=0;
		}
		else if(link_end==pos)
		{
			link_end=link_end->pre;
			link_end->next=0;
			
		}
		else
		{
			POS tmp=pos->pre;
			tmp->next=pos->next;
			tmp=pos->next;
			tmp->pre=pos->pre;
		}
		--total_element;
		ExFreePool(pos);
	}
	unsigned int size()
	{
		return total_element;
	}



	POS begin()
	{
		return link_first;
	}
	POS end()
	{
		return link_end;
	}
	POS next(POS& pos)
	{
		pos=pos->next;
		return pos;
	}
	POS previous(POS& pos)
	{
		pos=pos->pre;
		return pos;
	}
	bool empty()
	{
		return link_first==0;
	}
};
