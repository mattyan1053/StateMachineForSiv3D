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
	/// デフォルトコンストラクタ
	/// </summary>
	/// <remarks> 必ず状態IDをつけます。 </remarks>
	State() = delete;

	/// <summary>
	/// 新しい状態を作成します。
	/// </summary>
	/// <param name="id"> 状態ID </param>
	explicit State(T id) :m_id(id) {}

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	virtual ~State() = default;

	/// <summary>
	/// 状態IDを取得します。
	/// </summary>
	const T& Id() { return m_id; }

	/// <summary>
	/// 状態に入ったときに呼ばれます。
	/// </summary>
	virtual void setUp() {}

	/// <summary>
	/// 情報を更新します。
	/// </summary>
	/// <remarks> 毎フレーム呼ばれます。 </remarks>
	virtual void update() {}

	/// <summary>
	/// 描画を更新します。
	/// </sumamry>
	/// <remarks> 毎フレーム呼ばれます。 </remarks>
	virtual void draw() const {}

	/// <summary>
	/// 次の状態に移る前に呼ばれます。
	/// </summary>
	virtual void cleanUp() {}

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
	HashTable<T, std::shared_ptr<State<T>>> m_stateList;

	/// <summary>
	/// 現在の状態
	/// </summary>
	std::shared_ptr<State<T>> m_state;

public:

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	StateMachine() = default;

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	virtual ~StateMachine() = default;

	/// <summary>
	/// 現在の状態のIDを返します。
	/// </summary>
	/// <returns> 現在の状態のID </returns>
	Optional<const T&> getCurrentStateName() const {
		if (m_state == nullptr) {
			return none;
		}
		return m_state->Id();
	}

	/// <summary>
	/// 指定の状態へ移行します。
	/// </summary>
	/// <param name="nextStateId"> 移行先の状態ID </param>
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
	/// ステートマシンの初期化を行います。
	/// </summary>
	/// <remarks> 必ずオーバーライドします。 </remarks>
	virtual void initializeStateMachine() = 0;

	/// <summary>
	/// 状態を追加します。
	/// </summary>
	/// <param name="state"> 追加する状態のstd::make_shared</param>
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
	/// 毎フレーム呼ばれます。情報を更新します。
	/// </summary>
	void update() {
		if (m_state == nullptr) {
			return;
		}
		m_state->update();
	}

	/// <summary>
	/// 毎フレーム呼ばれます。描画を更新します。
	/// </summary>
	void draw() const {
		if (m_state == nullptr) {
			return;
		}
		m_state->draw();
	}

};