# pragma once
# include <Siv3D.hpp>

/// <summary>
/// ��Ԋ��N���X
/// </summary>
template<typename T>
class State {
private:

	T m_id;

public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	/// <remarks> �K�����ID�����܂��B </remarks>
	State() = delete;

	/// <summary>
	/// �V������Ԃ��쐬���܂��B
	/// </summary>
	/// <param name="id"> ���ID </param>
	explicit State(T id) :m_id(id) {}

	/// <summary>
	/// �f�t�H���g�f�X�g���N�^
	/// </summary>
	virtual ~State() = default;

	/// <summary>
	/// ���ID���擾���܂��B
	/// </summary>
	const T& Id() { return m_id; }

	/// <summary>
	/// ��Ԃɓ������Ƃ��ɌĂ΂�܂��B
	/// </summary>
	virtual void setUp() {}

	/// <summary>
	/// �����X�V���܂��B
	/// </summary>
	/// <remarks> ���t���[���Ă΂�܂��B </remarks>
	virtual void update() {}

	/// <summary>
	/// �`����X�V���܂��B
	/// </sumamry>
	/// <remarks> ���t���[���Ă΂�܂��B </remarks>
	virtual void draw() const {}

	/// <summary>
	/// ���̏�ԂɈڂ�O�ɌĂ΂�܂��B
	/// </summary>
	virtual void cleanUp() {}

};

/// <summary>
/// ��ԊǗ��N���X
/// </summary>
template<typename T>
class StateMachine {

private:

	/// <summary>
	/// ��ԃ��X�g
	/// </summary>
	HashTable<T, std::shared_ptr<State<T>>> m_stateList;

	/// <summary>
	/// ���݂̏��
	/// </summary>
	std::shared_ptr<State<T>> m_state;

public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	StateMachine() = default;

	/// <summary>
	/// �f�t�H���g�f�X�g���N�^
	/// </summary>
	virtual ~StateMachine() = default;

	/// <summary>
	/// ���݂̏�Ԃ�ID��Ԃ��܂��B
	/// </summary>
	/// <returns> ���݂̏�Ԃ�ID </returns>
	Optional<const T&> getCurrentStateName() const {
		if (m_state == nullptr) {
			return none;
		}
		return m_state->Id();
	}

	/// <summary>
	/// �w��̏�Ԃֈڍs���܂��B
	/// </summary>
	/// <param name="nextStateId"> �ڍs��̏��ID </param>
	void goToState(T nextStateId) {

		if (!m_stateList.contains(nextStateId)) {
			Print << U"Error: Not exist state: " << nextStateId;
			return;
		}

		if (m_state != nullptr) {
			m_state->cleanUp();
		}

		m_state = m_stateList[nextStateId];
		m_state->setUp();

	}

	/// <summary>
	/// �X�e�[�g�}�V���̏��������s���܂��B
	/// </summary>
	/// <remarks> �K���I�[�o�[���C�h���܂��B </remarks>
	virtual void initializeStateMachine() = 0;

	/// <summary>
	/// ��Ԃ�ǉ����܂��B
	/// </summary>
	/// <param name="state"> �ǉ������Ԃ�std::make_shared</param>
	void addState(const std::shared_ptr<State<T>>& state) {
		if (state == nullptr) {
			Print << U"Error: This state is nullptr";
			return;
		}
		if (m_stateList.contains(state->Id())) {
			Print << U"Error: Already exist state: " << state->Id();
			return;
		}
		m_stateList[state->Id()] = state;
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�����X�V���܂��B
	/// </summary>
	void update() {
		if (m_state == nullptr) {
			return;
		}
		m_state->update();
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�`����X�V���܂��B
	/// </summary>
	void draw() const {
		if (m_state == nullptr) {
			return;
		}
		m_state->draw();
	}

};