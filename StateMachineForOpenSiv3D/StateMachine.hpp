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
	State(T id) :m_id(id) {}

	/// <summary>
	/// ���ID���擾���܂��B
	/// </summary>
	T Id() { return m_id; }

	/// <summary>
	/// ��Ԃɓ������Ƃ��ɌĂ΂�܂��B
	/// </summary>
	virtual void setUp() {};

	/// <summary>
	/// �����X�V���܂��B
	/// </summary>
	/// <remarks> ���t���[���Ă΂�܂��B </remarks>
	virtual void update() {};

	/// <summary>
	/// �`����X�V���܂��B
	/// </sumamry>
	/// <remarks> ���t���[���Ă΂�܂��B </remarks>
	virtual void draw() const {};

	/// <summary>
	/// ���̏�ԂɈڂ�O�ɌĂ΂�܂��B
	/// </summary>
	virtual void cleanUp() {};

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
	std::unordered_map<T, std::shared_ptr<State<T>>> m_stateList;

	/// <summary>
	/// ���݂̏��
	/// </summary>
	std::shared_ptr<State<T>> m_state;

public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	StateMachine() {	}

	/// <summary>
	/// ���݂̏�Ԃ�Ԃ��܂��B
	/// </summary>
	/// <returns> ���݂̏�� </returns>
	State<T> currentState() {
		return *m_state;
	}

	/// <summary>
	/// �w��̏�Ԃֈڍs���܂��B
	/// </summary>
	/// <param name="nextStateId"> �ڍs��̏��ID </param>
	void goToState(T nextStateId) {

		if (!m_stateList.contains(nextStateId)) {
			Print << U"Error: Not exist stete: " << nextStateId;
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
	/// <param name="state"> �ǉ������� </param>
	void addState(State<T> *state) {
		std::shared_ptr<State<T>> s(state);
		if (s == nullptr) {
			Print << U"Error: This state is nullptr";
			return;
		}
		if (m_stateList.contains(s->Id())) {
			Print << U"Error: Already exist state: " << s->Id();
			return;
		}
		m_stateList[s->Id()] = s;
	}

	/// <summary>
	/// ���݂̏�Ԃ��w��̏�Ԃ��ǂ������ׂ܂��B
	/// </summary>
	/// <param name="stateId"> ���݂̏�ԂƔ�r������ </param>
	/// <returns> ���݂̏�Ԃ��w��̏�Ԃƈ�v���邩�ǂ����̐^�U </returns>
	bool isStateSame(T stateId) {
		if (m_state == nullptr) return false;
		return m_state->Id() == stateId;
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�����X�V���܂��B
	/// </summary>
	void update() {
		if (m_state != nullptr) {
			m_state->update();
		}
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�`����X�V���܂��B
	/// </summary>
	void draw() const {
		if (m_state != nullptr) {
			m_state->draw();
		}
	}

};