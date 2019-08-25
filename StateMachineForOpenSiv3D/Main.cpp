
# include <Siv3D.hpp> // OpenSiv3D v0.4.0

# include "StateMachine.hpp"

// 状態
// 列挙体を使っているがStringなどでも良い
enum BallStates {
	Idle,
	Moving,
};

// 状態管理したいクラス
class MovingBall : public StateMachine<BallStates> {

	Circle m_ball;

public:

	MovingBall() {
		m_ball.r = 40;
		m_ball.setPos({ 100, 200 });
		initializeStateMachine();
		goToState(BallStates::Idle);
	}

	// ステートマシンを初期化する。
	virtual void initializeStateMachine() override {

		// 状態を追加していく。

		// idle
		{
			auto state = new Idle(*this);
			addState(state);
		}

		// moving
		{
			auto state = new Moving(*this);
			addState(state);
		}
	}

	// 状態をインナークラスで定義する。
	struct Idle : public State<BallStates> {
		// 外側クラスを覚えておくとアクセスできる
		MovingBall& main;
		Idle(MovingBall &_main) :State<BallStates>(BallStates::Idle), main(_main) {}

		// 状態移行直後
		virtual void setUp() override {
			main.m_ball.x = 100;
			Print << U"Start Idle";
		}
		// 状態の最中
		virtual void update() override {
			if (MouseL.down()) {
				main.goToState(BallStates::Moving);
			}
		}
		virtual void draw() const override {
			main.m_ball.draw(Palette::Blue);
		}
		// 状態終了時
		virtual void cleanUp() override {
			Print << U"Finish Idle";
		}
	};

	struct Moving : public State<BallStates> {
		MovingBall& main;
		Moving(MovingBall& _main) :State<BallStates>(BallStates::Moving), main(_main) {}

		virtual void setUp() override {
			Print << U"Start Moving";
		}
		virtual void update() override {
			main.m_ball.x += 10;
			if (main.m_ball.x >= 500) {
				main.goToState(BallStates::Idle);
			}
		}
		virtual void draw() const override {
			main.m_ball.draw(Palette::Red);
		}
		virtual void cleanUp() override {
			Print << U"Finish Moving";
		}
	};

};

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));

	// 状態を持つオブジェクト
	MovingBall b;

	while (System::Update())
	{

		b.update();
		b.draw();

	}
}
