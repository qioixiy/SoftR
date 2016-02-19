#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <map>
#include <list>
class RBObject;
class RBEffect;

#pragma warning(disable : 4005)

/*
//useless
template<class T>
struct TypeTraits
{
	typedef typename T::vertex_format type;
};
*/

void RBopen_log();
void RBlog(const char* strbuf);
void RBclose_log();

struct EffectObjects
{
	typedef std::map < RBObject*, RBObject* > _SingleList;
	typedef std::map<RBEffect*, std::map<RBObject*, RBObject* >* > _EffectObjects;
	_EffectObjects pairs;

	void add(RBEffect* effect, RBObject* obj)
	{
		if (!effect) return;
		_EffectObjects::iterator it;
		it = pairs.find(effect);
		if (pairs.end() == it)
		{
			pairs[effect] = new _SingleList();
		}
		(*pairs[effect])[obj] = obj;
	}




};

class RBSem
{
public:
	RBSem(int c = 0) :count(c) {}
	//wait
	void wait()
	{
		std::unique_lock<std::mutex> lock(mx);
		if (--count<0)
		{
			cv.wait(lock, [&]()->bool {return count >= 0; });
		}
	}
	//signal
	void signal()
	{
		std::lock_guard<std::mutex> lock(mx);
		++count;
		cv.notify_one();

	}

private:
	int count;
	std::condition_variable cv;
	std::mutex mx;

};