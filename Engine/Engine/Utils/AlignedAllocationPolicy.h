#ifndef _ALIGNEDALLOCATIONPOLICY_H_
#define _ALIGNEDALLOCATIONPOLICY_H_

// warning c4316 ó����
// XMATRIX�� ���Ǹ� ���캸�� 16����Ʈ ������ ����Ѵٰ� ���
// XMATIRX�� ���� Ŭ������ ���� �Ҵ�Ǹ� 8����Ʈ �Ҵ�Ǳ� ������
// ���� �߻��� -> ���� ������ ������ ���� �� �ִ�
// �׷��� ������ 16����Ʈ �Ҵ�ǰ� ó���ϴ� Ŭ������ �����ؼ� ������ ������
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
