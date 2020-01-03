#pragma once

#include <mutex>
#include <tl/containers/vector.hpp>
#include <tl/swap.hpp>

namespace tl
{

	// thread safe work quueue with fixed maximum size
	template <typename T>
		class QueueFixedTS
	{
		const int CAP;
        T* a;
		int i, j;
		mutable std::mutex mut;
	public:
		QueueFixedTS(int CAPACITY = 32)
			: CAP(CAPACITY)
            , a(new T[CAPACITY])
		{
			i = j = 0;
		}
		bool pop(T& out) {
			std::lock_guard<std::mutex> lock(mut);
            if(i == j)
                return false;
            out = tl::move(a[i]);
			i = (i + 1) % CAP;
			return true;
		}
        void push(const T& in) {
            std::lock_guard<std::mutex> lock(mut);
            assert(j - i < CAP);
            a[j] = T(in);
            j = (j + 1) % CAP;
        }
        void push(T&& in) {
			std::lock_guard<std::mutex> lock(mut);
			assert(j - i < CAP);
            a[j] = tl::move(in);
			j = (j + 1) % CAP;
		}
        void swap(QueueFixedTS& o) {
            std::lock_guard<std::mutex> myLock(mut);
            std::lock_guard<std::mutex> otherLock(o.mut);
            tl::swap(i, o.i);
            tl::swap(j, o.j);
            tl::swap(a, o.a);
        }
		~QueueFixedTS() {
			i = j = 0;
            delete[] a;
		}
	};

}
