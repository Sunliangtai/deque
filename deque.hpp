#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include<iostream>
#include <cstddef>
namespace sjtu {
	template<class T>
	class deque {
	public:
		size_t len;
		class list {
			friend class deque;
		public:
			list(int size) :sizeoflist(size), currentSize(0), next(NULL), pre(NULL), data(NULL) {
				data = new T*[size];
			}
			list(const list& other) :sizeoflist(other.sizeoflist), currentSize(other.currentSize), next(NULL), pre(NULL), data(NULL) {
				data = new T*[sizeoflist];
				for (int i = 0; i < other.currentSize; i++) data[i] = new T(*other.data[i]);
			}
			~list() {
				for (int i = 0; i < currentSize; i++) delete data[i];
				delete[] data;
			}

			int currentSize;
		private:
			int sizeoflist;
			T** data;
			list *next, *pre;
		};
		class const_iterator;
		class iterator {
			friend class deque;
			friend class const_iterator;
		private:
			const deque* d;
			list* l;
			int num;
		public:
			iterator() :d(NULL), l(NULL), num(0) {}
			iterator(const deque* deq, list* b, int p = 0) :d(deq), l(b), num(p) {}
			iterator(const iterator& rhs) :d(rhs.d), l(rhs.l), num(rhs.num) {}
			iterator operator+(const int &n) const {
				if (n < 0) return operator-(-n);
				if (n == 0) return *this;
				int cnt = n;
				list *newl = l;
				int newnum = num;
				while (newl->currentSize - newnum <= cnt) {
					if (newl->next->next == nullptr) break;
					cnt -= newl->currentSize - newnum;
					newnum = 0;
					newl = newl->next;
				}
				if (newl->next == nullptr) throw "index_out_of_bound";
				newnum += cnt;
				return iterator(d, newl, newnum);
			}
			iterator operator-(const int &n) const {
				if (n < 0) return operator+(-n);
				if (n == 0) return *this;
				list *newl = l;
				int newnum = num;
				int cnt = n;
				while (newnum < cnt) {
					if (newl->pre == nullptr) break;
					cnt -= newnum + 1;
					newl = newl->pre;
					newnum = newl->currentSize - 1;
				}
				if (newnum < cnt && newl->pre == nullptr) throw "index_out_of_bound";//get to the begin and want to get pre
				newnum -= cnt;
				return iterator(d, newl, newnum);
			}

			int operator-(const iterator &rhs) const {
				if (d != rhs.d) throw "invalid_iterator";
				if (l == rhs.l) return num - rhs.num;
				list* newl = l->next;
				int ans = l->currentSize - num;
				while (newl != nullptr && newl != rhs.l) {
					ans += newl->currentSize;
					newl = newl->next;
				}
				if (newl != nullptr) return -(ans + rhs.num);
				ans = rhs.l->currentSize - rhs.num;
				newl = rhs.l->next;
				while (newl != nullptr && newl != l) {
					ans += newl->currentSize;
					newl = newl->next;
				}
				if (newl != nullptr) return (ans + num);
				throw "runtime_error";
			}
			iterator operator+=(const int &n) {
				*this = (*this) + n;
				return *this;
			}
			iterator operator-=(const int &n) {
				*this = (*this) - n;
				return *this;
			}

			iterator operator++(int) {
				iterator old(*this);
				(*this) += 1;
				return old;
			}

			iterator& operator++() {
				if (num == l->currentSize - 1) {
					num = 0;
					if (l->next == NULL) throw "index_out_of_bound";
					l = l->next;
				}
				else {
					++num;
				}
				return *this;
			}

			iterator operator--(int) {
				iterator old(*this);
				--(*this);
				return old;
			}

			iterator& operator--() {
				if (num == 0) {
					if (l->pre == NULL) throw "index_out_of_bound";
					l = l->pre;
					num = l->currentSize - 1;
				}
				else {
					--num;
				}
				return *this;
			}

			T& operator*() const {
				if (num < 0 || num >= l->currentSize) throw "index_out_of_bound";
				return *(l->data[num]);
			}

			T* operator->() const noexcept { return l->data[num]; }

			bool operator==(const iterator &rhs) const {
				return (l == rhs.l && num == rhs.num) || (l->next == nullptr && rhs.l->next == l && rhs.num == rhs.l->currentSize) || (rhs.l->next ==
					nullptr && l->next == rhs.l && num == l->currentSize);
			}
			bool operator==(const const_iterator &rhs) const {
				return (l == rhs.l && num == rhs.num) || (l->next == nullptr && rhs.l->next == l && rhs.num == rhs.l->currentSize) || (rhs.l->next ==
					nullptr && l->next == rhs.l && num == l->currentSize);
			}

			bool operator!=(const iterator &rhs) const {
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const {
				return !(*this == rhs);
			}
		};

		class const_iterator {
			friend class deque;
			friend class iterator;

		private:
			const deque* d;
			list* l;
			int num;
		public:
			const_iterator() :d(NULL), l(NULL), num(0) {}
			const_iterator(const deque* deq, list* b, int p = 0) :d(deq), l(b), num(p) {}
			const_iterator(const const_iterator& rhs) :d(rhs.d), l(rhs.l), num(rhs.num) {}
			const_iterator(const iterator& rhs) :d(rhs.d), l(rhs.l), num(rhs.num) {}
			const_iterator operator+(const int &n) const {
				if (n < 0) return operator-(-n);
				if (n == 0) return *this;
				int cnt = n;
				list *newl = l;
				int newnum = num;
				while (newl->currentSize - newnum <= cnt) {
					if (newl->next->next == nullptr) break;
					cnt -= newl->currentSize - newnum;
					newnum = 0;
					newl = newl->next;
				}
				if (newl->next == nullptr) throw "index_out_of_bound";
				newnum += cnt;
				return const_iterator(d, newl, newnum);
			}
			const_iterator operator-(int n) const {
				if (n < 0) return operator+(-n);
				if (n == 0) return *this;
				list *newl = l;
				int newnum = num;
				int cnt = n;
				while (newnum < cnt) {
					if (newl->pre == nullptr) break;
					cnt -= newnum + 1;
					newl = newl->pre;
					newnum = newl->currentSize - 1;
				}
				if (newnum < cnt && newl->pre == nullptr) throw "index_out_of_bound";
				newnum -= cnt;
				return const_iterator(d, newl, newnum);
			}

			int operator-(const const_iterator &rhs) const {
				if (d != rhs.d) throw "invalid_iterator";
				if (l == rhs.l) return num - rhs.num;
				list* newl = l->next;
				int ans = l->currentSize - num;
				while (newl != nullptr && newl != rhs.l) {
					ans += newl->currentSize;
					newl = newl->next;
				}
				if (newl != nullptr) return -(ans + rhs.num);
				ans = rhs.l->currentSize - rhs.num;
				newl = rhs.l->next;
				while (newl != nullptr && newl != l) {
					ans += newl->currentSize;
					newl = newl->next;
				}
				if (newl != nullptr) return (ans + num);
				throw "runtime_error";
			}

			const_iterator operator+=(const int &n) {
				*this = (*this) + n;
				return *this;
			}

			const_iterator operator-=(const int &n) {
				*this = (*this) - n;
				return *this;
			}

			const_iterator operator++(int) {
				const_iterator old(*this);
				++(*this);
				return old;
			}

			const_iterator& operator++() {
				(*this) += 1;
				return (*this);
			}

			const_iterator operator--(int) {
				const_iterator old(*this);
				--(*this);
				return old;
			}

			const_iterator& operator--() {
				(*this) -= 1;
				return (*this);
			}

			T& operator*() const {
				if (num < 0 || num >= l->currentSize) throw "index_out_of_bound";
				return *(l->data[num]);
			}

			T* operator->() const noexcept { return l->data[num]; }

			bool operator==(const iterator &rhs) const {
				return (l == rhs.l && num == rhs.num) ||
					(l->next == nullptr && rhs.l->next == l && rhs.num == rhs.l->currentSize) ||
					(rhs.l->next == nullptr && l->next == rhs.l && num == l->currentSize);
			}
			bool operator==(const const_iterator &rhs) const {
				return (l == rhs.l && num == rhs.num) ||
					(l->next == nullptr && rhs.l->next == l && rhs.num == rhs.l->currentSize) ||
					(rhs.l->next == nullptr && l->next == rhs.l && num == l->currentSize);
			}

			bool operator!=(const iterator &rhs) const {
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const {
				return !(*this == rhs);
			}
		};
		deque() :ksize(1000), len(0), head(NULL), tail(NULL) {
			head = new list(1000);
			tail = new list(1);
			head->next = tail;
			tail->pre = head;
		}
		deque(const deque &other) :ksize(other.ksize), len(other.len), head(NULL), tail(NULL) {
			head = new list(*other.head);
			tail = new list(1);
			list* newl = head;
			for (list* ite = other.head->next; ite != other.tail; ite = ite->next) {
				newl->next = new list(*ite);
				newl->next->pre = newl;
				newl = newl->next;
			}
			newl->next = tail;
			tail->pre = newl;
		}
		~deque() {
			clear();
			delete head;
			delete tail;
		}
		deque &operator=(const deque &other) {
			if (this == &other) return *this;
			clear();
			len = other.len;
			delete head;
			head = new list(*other.head);
			list* newl = head;
			for (list* ite = other.head->next; ite != other.tail; ite = ite->next) {
				newl->next = new list(*ite);
				newl->next->pre = newl;
				newl = newl->next;
			}
			newl->next = tail;
			tail->pre = newl;
			return *this;
		}

		T & at(const size_t &pos) {
			list* newl = head;
			int num = pos;
			while (newl != tail && num >= newl->currentSize) {
				num -= newl->currentSize;
				newl = newl->next;
			}
			if (newl == tail) throw "index_out_of_bound";
			if (num < 0 || num >= newl->currentSize) throw "index_out_of_bound";
			return *(newl->data[num]);
		}
		const T & at(const size_t &pos) const {
			list* newl = head;
			int num = pos;
			while (newl != tail && num >= newl->currentSize) num -= newl->currentSize, newl = newl->next;
			if (newl == tail) throw "index_out_of_bound";
			if (num < 0 || num >= newl->currentSize) throw "index_out_of_bound";
			return *(newl->data[num]);
		}
		T & operator[](const size_t &pos) {
			return at(pos);
		}
		const T & operator[](const size_t &pos) const {
			return at(pos);
		}

		const T & front() const {
			if (len == 0) throw "container_is_empty";
			return *(head->data[0]);
		}

		const T & back() const {
			if (len == 0) throw "container_is_empty";
			return *(tail->pre->data[tail->pre->currentSize - 1]);
		}

		iterator begin() {
			if (len == 0) return end();
			return iterator(this, head, 0);
		}
		const_iterator cbegin() const {
			if (len == 0) return cend();
			return const_iterator(this, head, 0);
		}

		iterator end() { return iterator(this, tail, 0); }
		const_iterator cend() const { return const_iterator(this, tail, 0); }


		bool empty() const { return len == 0; }

		size_t size() const { return len; }

		void clear() {
			list* newl = head->next;
			list* tmp;
			delete head;
			head = new list(1000);
			head->next = tail;
			tail->pre = head;
			while (newl != tail) {
				tmp = newl->next;
				delete newl;
				newl = tmp;
			}
			len = 0;
		}

		iterator insert(iterator pos, const T &value) {
			if (this != pos.d) throw "invalid_iterator";
			if (pos.num == 0 && pos.l->pre != nullptr) {
				pos = iterator(this, pos.l->pre, pos.l->pre->currentSize);
			}
			++len;
			merge(pos.l);
			split(pos.l);
			while (pos.l->next != nullptr && pos.num > pos.l->currentSize) {
				pos.num -= pos.l->currentSize;
				pos.l = pos.l->next;
			}
			if (pos.l->next == nullptr) throw "index_out_of_bound";
			if (pos.num == pos.l->currentSize) {
				pos.l->data[pos.l->currentSize] = new T(value);
				++pos.l->currentSize;
				return pos;
			}
			for (int i = pos.l->currentSize; i > pos.num; i--) {
				pos.l->data[i] = pos.l->data[i - 1];
			}
			pos.l->data[pos.num] = new T(value);
			pos.l->currentSize++;
			return pos;
		}

		iterator erase(iterator pos) {
			if (this != pos.d) throw "invalid_iterator";
			if (pos.l == tail) throw "invalid_iterator";
			--len;
			while (pos.num < 0) {
				pos.l = pos.l->pre;
				pos.num += pos.l->currentSize;
			}
			if (pos.num == pos.l->currentSize) {
				pos.num = 0;
				pos.l = pos.l->next;
			}
			merge(pos.l);
			split(pos.l);
			while (pos.l->next != nullptr && pos.num >= pos.l->currentSize) {
				pos.num -= pos.l->currentSize;
				pos.l = pos.l->next;
			}
			if (pos.l->next == nullptr) throw "index_out_of_bound";
			if (pos.num == pos.l->currentSize - 1) {
				delete pos.l->data[pos.l->currentSize - 1];
				--pos.l->currentSize;
				if (pos.num == pos.l->currentSize) {
					pos.num = 0;
					pos.l = pos.l->next;
				}
				return pos;
			}
			delete pos.l->data[pos.num];
			for (int i = pos.num; i < pos.l->currentSize - 1; i++) {
				pos.l->data[i] = pos.l->data[i + 1];
			}
			--pos.l->currentSize;
			if (pos.num == pos.l->currentSize) {
				pos.num = 0;
				pos.l = pos.l->next;
			}
			return pos;
		}

		void push_back(const T &value) { insert(iterator(this, tail->pre, tail->pre->currentSize), value); }

		void pop_back() { if (len == 0) throw "container_is_empty"; erase(iterator(this, tail->pre, tail->pre->currentSize - 1)); }

		void push_front(const T &value) { insert(begin(), value); }

		void pop_front() { if (len == 0) throw "container_is_empty"; erase(begin()); }

	private:
		int ksize = 1000;
		list* head;
		list* tail;
		void merge(list*& l) {
			if (l == tail) return;
			if (l->currentSize < ksize / 2) {
				while (l->currentSize + l->next->currentSize <= ksize) {
					if (l->next->next == nullptr) break;
					while (l->currentSize == 0) {
						if (l->next->next == nullptr) break;
						list *tmp = l;
						l = l->next;
						delete tmp;
					}
					list *newl = l->next;
					for (int i = l->currentSize; i < l->currentSize + newl->currentSize; i++) {
						l->data[i] = newl->data[i - l->currentSize];
					}
					l->currentSize += l->next->currentSize;
					newl->currentSize = 0;
					l->next = l->next->next;
					l->next->pre = l;
					delete newl;
				}
			}
		}
		void split(list *&l) {
			if (l == tail) return;
			if (l->currentSize >= ksize) {
				list* newl = new list(l->sizeoflist);
				newl->pre = l;
				newl->next = l->next;
				l->next->pre = newl;
				l->next = newl;
				int newnum = l->currentSize / 2;
				for (int i = newnum; i < l->currentSize; i++) {
					newl->data[i - newnum] = l->data[i];
				}
				newl->currentSize = l->currentSize - newnum;
				l->currentSize = newnum;
			}
		}
	};

}

#endif
