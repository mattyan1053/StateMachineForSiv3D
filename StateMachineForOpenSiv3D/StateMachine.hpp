# pragma once
# include <unordered_map>
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
	virtual void setUp() = 0;

	/// <summary>
	/// ���t���[���Ă΂�܂��B�����X�V���܂��B
	/// </summary>
	virtual void update() = 0;

	/// <summary>
	/// ���t���[���Ă΂�܂��B�`����X�V���܂��B
	/// </sumamry>
	virtual void draw() const = 0;

	/// <summary>
	/// ���̏�ԂɈڂ�O�ɌĂ΂�܂��B
	/// </summary>
	virtual void cleanUp() = 0;

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
	std::unordered_map<T, std::shared_ptr<State<T>>> stateList;

	/// <summary>
	/// ���݂̏��
	/// </summary>
	std::shared_ptr<State<T>> state;

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
		return *state;
	}

	/// <summary>
	/// �w��̏�Ԃֈڍs���܂��B
	/// </summary>
	/// <param name="nextStateId"> �ڍs��̏��ID </param>
	void goToState(T nextStateId) {

		if (!stateList.contains(nextStateId)) {
			Print << U"Error: Not exist stete: " << nextStateId;
			return;
		}

		if (state != nullptr) {
			state->cleanUp();
		}

		state = stateList[nextStateId];
		state->setUp();

	}

	/// <summary>
	/// �X�e�[�g�}�V���̏��������s���܂��B
	/// </summary>
	virtual void initializeStateMachine() = 0;

	/// <summary>
	/// ��Ԃ�ǉ����܂��B
	/// </summary>
	void addState(State<T> *s) {
		if (s == nullptr) {
			Print << U"Error: This state is nullptr";
			return;
		}
		if (stateList.contains(s->Id())) {
			Print << U"Error: Already exist state: " << s->Id();
			return;
		}
		stateList[s->Id()] = std::shared_ptr<State<T>>(s);
	}

	/// <summary>
	/// ���݂̏�Ԃ��w��̏�Ԃ��ǂ������ׂ܂��B
	/// </summary>
	/// <param name="stateId"> ���݂̏�ԂƔ�r������ </param>
	/// <returns> ���݂̏�Ԃ��w��̏�Ԃƈ�v���邩�ǂ����̐^�U </returns>
	bool isStateSame(T stateId) {
		if (state == nullptr) return false;
		return state->Id() == stateId;
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�����X�V���܂��B
	/// </summary>
	void update() {
		if (state != nullptr) {
			state->update();
		}
	}

	/// <summary>
	/// ���t���[���Ă΂�܂��B�`����X�V���܂��B
	/// </summary>
	void draw() const {
		if (state != nullptr) {
			state->draw();
		}
	}

};