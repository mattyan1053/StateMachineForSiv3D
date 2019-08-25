# pragma once
# include <Siv3D.hpp>

/// <summary>
/// 状態基底クラス
/// </summary>
template<typename T>
class State {
private:

	T m_id;

public:

	/// <summary>
	/// 新しい状態を作成します。
	/// </summary>
	/// <param name="id"> 状態ID </param>
	State(T id) :m_id(id) {}

	/// <summary>
	/// 状態IDを取得します。
	/// </summary>
	T Id() { return m_id; }

	/// <summary>
	/// 状態に入ったときに呼ばれます。
	/// </summary>
	virtual void setUp() = 0;

	/// <summary>
	/// 毎フレーム呼ばれます。情報を更新します。
	/// </summary>
	virtual void update() = 0;

	/// <summary>
	/// 毎フレーム呼ばれます。描画を更新します。
	/// </sumamry>
	virtual void draw() const = 0;

	/// <summary>
	/// 次の状態に移る前に呼ばれます。
	/// </summary>
	virtual void cleanUp() = 0;

};

/// <summary>
/// 状態管理クラス
/// </summary>
template<typename T>
class StateMachine {

private:

	/// <summary>
	/// 状態リスト
	/// </summary>
	std::unordered_map<T, std::shared_ptr<State<T>>> m_stateList;

	/// <summary>
	/// 現在の状態
	/// </summary>
	std::shared_ptr<State<T>> m_state;

public:

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	StateMachine() {	}

	/// <summary>
	/// 現在の状態を返します。
	/// </summary>
	/// <returns> 現在の状態 </returns>
	State<T> currentState() {
		return *m_state;
	}

	/// <summary>
	/// 指定の状態へ移行します。
	/// </summary>
	/// <param name="nextStateId"> 移行先の状態ID </param>
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
	/// ステートマシンの初期化を行います。
	/// </summary>
	virtual void initializeStateMachine() = 0;

	/// <summary>
	/// 状態を追加します。
	/// </summary>
	/// <param name="state"> 追加する状態 </param>
	void addState(State<T> *state) {
		if (state == nullptr) {
			Print << U"Error: This state is nullptr";
			return;
		}
		if (m_stateList.contains(state->Id())) {
			Print << U"Error: Already exist state: " << state->Id();
			return;
		}
		m_stateList[state->Id()] = std::shared_ptr<State<T>>(state);
	}

	/// <summary>
	/// 現在の状態が指定の状態かどうか調べます。
	/// </summary>
	/// <param name="stateId"> 現在の状態と比較する状態 </param>
	/// <returns> 現在の状態が指定の状態と一致するかどうかの真偽 </returns>
	bool isStateSame(T stateId) {
		if (m_state == nullptr) return false;
		return m_state->Id() == stateId;
	}

	/// <summary>
	/// 毎フレーム呼ばれます。情報を更新します。
	/// </summary>
	void update() {
		if (m_state != nullptr) {
			m_state->update();
		}
	}

	/// <summary>
	/// 毎フレーム呼ばれます。描画を更新します。
	/// </summary>
	void draw() const {
		if (m_state != nullptr) {
			m_state->draw();
		}
	}

};