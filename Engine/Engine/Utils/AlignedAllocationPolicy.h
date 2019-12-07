#ifndef _ALIGNEDALLOCATIONPOLICY_H_
#define _ALIGNEDALLOCATIONPOLICY_H_

// warning c4316 처리용
// XMATRIX의 정의를 살펴보면 16바이트 정렬을 사용한다고 명시
// XMATIRX를 가진 클래스가 힙에 할당되면 8바이트 할당되기 때문에
// 워닝 발생함 -> 정렬 데이터 순서가 섞일 수 있다
// 그래서 강제로 16바이트 할당되게 처리하는 클래스를 생성해서 연산자 덮어씌운다
template<size_t T>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};


#endif // !_ALIGNEDALLOCATIONPOLICY_H_
