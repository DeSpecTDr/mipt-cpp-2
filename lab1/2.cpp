#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

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

class PenaltyDice : public Roll {
 public:
  PenaltyDice(Roll* dice) : dice_(dice) {}

  unsigned roll() override {
    unsigned roll1 = dice_->roll();
    unsigned roll2 = dice_->roll();
    return (roll1 < roll2) ? roll1 : roll2;
  }

 private:
  Roll* dice_;
};

class BonusDice : public Roll {
 public:
  BonusDice(Roll* dice) : dice_(dice) {}

  unsigned roll() override {
    unsigned roll1 = dice_->roll();
    unsigned roll2 = dice_->roll();
    return (roll1 > roll2) ? roll1 : roll2;
  }

 private:
  Roll* dice_;
};

double expected_value(Roll& rollable, unsigned number_of_rolls = 1000000) {
  auto accum = 0llu;
  for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
    accum += rollable.roll();
  }
  return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double value_probability(unsigned value, Roll& dice,
                         unsigned number_of_rolls = 100000) {
  unsigned count = 0;
  for (unsigned i = 0; i < number_of_rolls; ++i) {
    if (dice.roll() == value) {
      count++;
    }
  }
  return static_cast<double>(count) / static_cast<double>(number_of_rolls);
}

void output_histogram_data(Roll& dice, const std::string& name,
                           unsigned min_val, unsigned max_val,
                           unsigned rolls = 100000) {
  std::cout << "# " << name << " histogram data" << std::endl;
  std::cout << "# value,probability" << std::endl;

  for (unsigned i = min_val; i <= max_val; ++i) {
    double prob = value_probability(i, dice, rolls);
    if (prob > 0.0001) {
      std::cout << i << "," << prob << std::endl;
    }
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  Dice dice1(6, 1);
  Dice dice2(6, 2);
  Dice dice3(6, 3);
  Dice singleDice(6, 4);
  Dice bigDice(100, 5);
  ThreeDicePool threeDicePool(&dice1, &dice2, &dice3);

  PenaltyDice penaltyDice(&singleDice);
  BonusDice bonusDice(&singleDice);

  PenaltyDice penaltyThreeDice(&threeDicePool);
  BonusDice bonusThreeDice(&threeDicePool);

  PenaltyDice penaltyBigDice(&bigDice);
  BonusDice bonusBigDice(&bigDice);

  bool show_expected = false;
  bool show_all = false;
  bool show_big_normal = false;
  bool show_big_penalty = false;
  bool show_big_bonus = false;
  bool show_three_normal = false;
  bool show_three_penalty = false;
  bool show_three_bonus = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--expected") {
      show_expected = true;
    } else if (arg == "--big-normal") {
      show_big_normal = true;
    } else if (arg == "--big-penalty") {
      show_big_penalty = true;
    } else if (arg == "--big-bonus") {
      show_big_bonus = true;
    } else if (arg == "--three-normal") {
      show_three_normal = true;
    } else if (arg == "--three-penalty") {
      show_three_penalty = true;
    } else if (arg == "--three-bonus") {
      show_three_bonus = true;
    } else if (arg == "--all") {
      show_all = true;
    }
  }

  if (show_expected || show_all) {
    std::cout << "Обычный кубик [1,6]: " << expected_value(singleDice)
              << "\nШтраф кубик [1,6]: " << expected_value(penaltyDice)
              << "\nПреимущество кубик [1,6]: " << expected_value(bonusDice)
              << "\nТри кубика [1,6]: " << expected_value(threeDicePool)
              << "\nШтраф три кубика: " << expected_value(penaltyThreeDice)
              << "\nПреимущество три кубика: " << expected_value(bonusThreeDice)
              << "\nБольшой кубик [1,100]: " << expected_value(bigDice)
              << "\nШтраф большой кубик: " << expected_value(penaltyBigDice)
              << "\nПреимущество большой кубик: "
              << expected_value(bonusBigDice) << std::endl;
  }

  if (show_all || show_big_normal) {
    output_histogram_data(bigDice, "Big Dice Normal [1,100]", 1, 100);
  }
  if (show_all || show_big_penalty) {
    output_histogram_data(penaltyBigDice, "Big Dice Penalty [1,100]", 1, 100);
  }
  if (show_all || show_big_bonus) {
    output_histogram_data(bonusBigDice, "Big Dice Bonus [1,100]", 1, 100);
  }
  if (show_all || show_three_normal) {
    output_histogram_data(threeDicePool, "Three Dice Normal", 3, 18);
  }
  if (show_all || show_three_penalty) {
    output_histogram_data(penaltyThreeDice, "Three Dice Penalty", 3, 18);
  }
  if (show_all || show_three_bonus) {
    output_histogram_data(bonusThreeDice, "Three Dice Bonus", 3, 18);
  }

  return 0;
}
