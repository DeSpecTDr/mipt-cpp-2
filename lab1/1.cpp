#include <iostream>
#include <random>

class Roll {
 public:
  virtual unsigned roll() = 0;
  virtual ~Roll() = default;
};

class Dice : public Roll {
 public:
  Dice(unsigned max, unsigned seed) : dstr(1, max), reng(seed) {}

  unsigned roll() override { return dstr(reng); }

 private:
  std::uniform_int_distribution<unsigned> dstr;
  std::default_random_engine reng;
};

class ThreeDicePool : public Roll {
 public:
  ThreeDicePool(Dice* dice1, Dice* dice2, Dice* dice3)
      : dice1_(dice1), dice2_(dice2), dice3_(dice3) {}

  unsigned roll() override {
    return dice1_->roll() + dice2_->roll() + dice3_->roll();
  }

 private:
  Dice *dice1_, *dice2_, *dice3_;
};

double expected_value(Roll& rollable, unsigned number_of_rolls = 1000000) {
  auto accum = 0llu;
  for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
    accum += rollable.roll();
  }
  return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main() {
  Dice dice1(6, 1);
  Dice dice2(6, 2);
  Dice dice3(6, 3);

  ThreeDicePool threeDicePool(&dice1, &dice2, &dice3);

  Dice singleDice(6, 4);

  std::cout << "Один кубик: " << expected_value(singleDice) << std::endl;
  std::cout << "Три кубика: " << expected_value(threeDicePool) << std::endl;

  return 0;
}
