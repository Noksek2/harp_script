#pragma warning(disable: 4146)
#include <h_head.h>


template <typename T, const uint64_t RES_CNT_MAX = _64MB>
class MyMemStack {//성능 쓰레기같은 스택
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	void Init(uint32_t m_capa_cnt = _4KB) {
		m_len = 0u;
		m_capa = max(m_capa_cnt, _4KB / sizeof(T));
		//Harp_assert(m_capa_cnt * sizeof(T) <= RES_MAX, "아 씨발년아");
		//m_reserved = m_capa_cnt;
		m_stack = (T*)MEM_reserve(RES_CNT_MAX, sizeof(T), RES_CNT_MAX * sizeof(T));
		m_stack = (T*)MEM_commit(m_stack, m_capa, sizeof(T), RES_CNT_MAX * sizeof(T));
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {

			while (re_size > m_capa)
				m_capa <<= 1;
			if (m_capa > RES_CNT_MAX) {
				m_capa = RES_CNT_MAX;
			}
			m_stack = (T*)MEM_commit(m_stack, m_capa, sizeof(T), RES_CNT_MAX * sizeof(T));
		}
	}
	void push(const T& dat) {
		if (m_len >= m_capa) {
			reserve(m_len + 1);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		reserve(re_size);

		m_len = re_size;
	}
	inline T pop() {
		return m_stack[--m_len];
	}
	inline void onlypop() {
		m_len--;
	}
	inline T top() {
		return m_stack[m_len - 1];
	}
	inline T* back_ptr() {
		return &m_stack[m_len];
	}
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }
	~MyMemStack() {
		if (m_stack) {
			MEM_free(m_stack);
			m_stack = NULL;
		}
	}
};

//Init, ~
template <typename T, const uint64_t MEM_CNT_MAX = _64MB>
class MyStack {
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	void Init(uint32_t _m_capa = _4KB) {
		m_len = 0u;
		m_capa = _m_capa;
		m_stack = harp_calloc<T>(m_capa);
		//m_stack = (T*)MEM_commit(m_stack, m_capa);
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {
			while (re_size > m_capa)
				m_capa <<= 1;
			Harp_assert(m_capa <= MEM_CNT_MAX, "[ERROR] reserve more than MEM_MAX");
			T* newarr = harp_realloc<T>(m_stack, m_capa);
			Harp_assert(newarr != NULL, "[ERROR] realloc failed");
			m_stack = newarr;
		}

	}
	void push() {
		reserve(m_len + 1);
		m_len++;
	}
	void push(const T& dat) {
		if (m_len == m_capa) {
			reserve(m_len + 1);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		reserve(re_size);
		m_len = re_size;
	}

	inline const uint32_t size() { return m_len; }
	inline T pop() {
		return m_stack[--m_len];
	}
	inline void onlypop() {
		m_len--;
	}
	inline T top() {
		return m_stack[m_len - 1];
	}
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }

	inline T* back_ptr() {
		return &m_stack[m_len];
	}
	~MyStack() {
		if (m_stack) {
			free(m_stack);
			m_stack = NULL;
		}
	}
};

void harplist_test() {
	uint32_t test_cnt = _256MB;
	clock_t c_s, c_e;
	{
		c_s = clock();
		MyMemStack<int, _256MB>st;
		st.Init(_4KB);
		for (uint32_t i = 0; i < test_cnt; i++) {
			st.push(i);
		}
		c_e = clock();
		printf("memstack : %d ms", c_e - c_s);
	}
	Sleep(5000);
	{
		c_s = clock();
		MyStack<int, _256MB>st;
		st.Init(_4KB);
		for (volatile uint32_t i = 0; i < test_cnt; i++) {
			st.push(i);
		}
		c_e = clock();
		printf("stack : %d ms", c_e - c_s);
	}
	Sleep(5000);
	{
		c_s = clock();
		std::vector<int>st;
		st.reserve(_4KB);
		for (volatile uint32_t i = 0; i < test_cnt; i++) {
			st.emplace_back(i);
		}
		c_e = clock();
		printf("std::vector : %d ms", c_e - c_s);
	}
	Sleep(5000);
}
void hashtable_test();

//template<typename T>
//static inline uint32_t TinyHashTable_GetHash(const T&);
static inline uint32_t TinyHashTable_GetHash(const std::wstring& name) {
	uint32_t len = name.size();
	uint32_t hash = 0u;
	for (uint32_t idx = 0; idx < len; idx++)
	{
		hash ^= name[idx];
		hash *= 16777619; // FNV prime
	}
	if (hash < 2u) return 2u;
	return hash;
}
static inline uint32_t TinyHashTable_GetHash(const wchar* name) {
	uint32_t len = wcslen(name);
	uint32_t hash = 0u;
	for (uint32_t idx = 0; idx < len; idx++)
	{
		hash ^= name[idx];
		hash *= 16777619; // FNV prime
	}
	if (hash < 2u) return 2u;
	return hash;
}
static inline bool TinyHashTable_Compare(const std::wstring& name, const std::wstring& key) {
	return name == key;
}


template<typename K, typename V, const uint32_t CAPA = _4KB>
class TinyHashTable {
	uint32_t len;uint32_t _;
	K keymap[CAPA];
	V keyval[CAPA];
	void Init() {
		printf("size of %u <K, V, %u>", sizeof(TinyHashTable), CAPA);
		memset(this, 0, sizeof(TinyHashTable));
	}
	void Delete() {
		memset(this, 0, sizeof(TinyHashTable));
	}
	TinyHashTable() { Init(); }

public:
	const uint32_t Len()const { return len; }
	uint32_t keyhash[CAPA];
	static TinyHashTable* New() {
		TinyHashTable* tb = new TinyHashTable();
		return tb;
	}
	~TinyHashTable() { Delete(); }

	//template<typename K2 = K>
	//static uint32_t GetHash(const K&);
	//static bool Compare(const K&);

	const uint32_t find_idx(const K& name) const {
		uint32_t kh = TinyHashTable_GetHash(name);
		const uint32_t o_idx = kh % CAPA;
		uint32_t idx = o_idx;
		while (idx < CAPA) {
			if (keyhash[idx] == 0u) {//empty
				return -1u;
			}
			else if (keyhash[idx] == kh) {
				if (TinyHashTable_Compare(name, keymap[idx])) { return idx; }
			}
			idx++;
		}
		idx = 0;
		while (idx < o_idx) {
			if (keyhash[idx] == 0u) {//empty
				return -1u;
			}
			else if (keyhash[idx] == kh) {
				if (TinyHashTable_Compare(name, keymap[idx])) { return idx; }
			}
			idx++;
		}
		return -1u;
	}
	uint32_t find_empty_idx(const K& name, uint32_t& key) {
		uint32_t kh = TinyHashTable_GetHash(name);
		const uint32_t o_idx = kh % CAPA;
		uint32_t idx = o_idx;
		while (idx < CAPA) {
			if (keyhash[idx] == 0u) {
				key = kh;
				return idx;
			}
			idx++;
		}
		idx = 0;
		while (idx < o_idx) {
			if (keyhash[idx] == 0u) {
				key = kh;
				return idx;
			}
			idx++;
		}
		return -1u;
	}
	void Insert(const K& name, const V& val, const uint32_t kh, const uint32_t idx);
	void emplace(const K& name, const V& val) {
		if (len * 3 > CAPA * 2) return;
		uint32_t kh;
		uint32_t idx = find_empty_idx(name, kh);
		if (idx == -1u) return;
		TinyHashTable_Insert(this, name, val, kh, idx);
		
		len++;
	}
	const V* find(const K& name) const {
		const uint32_t idx = find_idx(name);
		if (idx == -1u) return NULL;
		return &keyval[idx];
	}
};

template<typename K = std::wstring, typename V>
static void TinyHashTable::Insert(const K& name, const V& val, const uint32_t kh, const uint32_t idx) {
	this->keymap[idx].resize(name.size());
	this->memcpy(&keymap[idx][0], &name[0], sizeof(wchar_t) * name.size());
	this->keyval[idx] = val;
	this->keyhash[idx] = kh;
}
void main() {
	const int CAPA = 123456;
	auto shit = TinyHashTable<std::wstring, int, CAPA>::New();
	clock_t t_s, t_e;

	puts("myhashtable");
	wstring buf;
	buf.resize(7);
	{
		t_s = clock();
		for (volatile int i = 0;i < CAPA * 2 / 3; i++) {
			_itow_s(i, &buf[0], 7, 10);
			shit->emplace(buf, (int)i);
		}
		t_e = clock();
		printf("insert %d\n", t_e - t_s);

		t_s = clock();
		volatile int failed = 0;
		for (volatile int i = 0;i < CAPA; i++) {
			_itow_s(i, &buf[0], 7, 10);
			if (shit->find(buf) == NULL) failed++;
		}
		t_e = clock();
		printf("find %d (%d/%d)\n", t_e - t_s, failed, shit->Len());

		delete shit;

	}
	puts("unoredered_map");

	{
		std::unordered_map<std::wstring, int> ush;
		t_s = clock();
		for (volatile int i = 0;i < CAPA * 2 / 3; i++) {
			_itow_s(i, &buf[0], 7, 10);
			ush.emplace(buf, i);
		}
		t_e = clock();
		printf("insert %d\n", t_e - t_s);


		t_s = clock();
		volatile int failed = 0;
		for (volatile int i = 0;i < CAPA; i++) {
			_itow_s(i, &buf[0], 7, 10);
			if (ush.find(buf) == ush.end()) failed++;
		}
		t_e = clock();
		printf("find %d (%d/%d)\n", t_e - t_s, failed, ush.size());

	}
	return;
}