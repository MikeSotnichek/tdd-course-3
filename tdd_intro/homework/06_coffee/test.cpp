/*
We have to develop a coffee machine with TDD. The machine uses the automated source of ingredients, which implements the interface ISourceOfIngredients.

Our machine should be able to produce the coffee accordingly to the next receipts:
- americano: water & coffee 1:2 or 1:3. Water temp 60C
- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
- latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
- marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty

We have 2 possible sizes of the cup:
- little 100 gram
- big 140 gram

Implement worked coffee machine using ISourceOfIngredients to controll the process of coffee production.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ISourceOfIngredients
{
public:
    virtual ~ISourceOfIngredients() {}
    virtual void SetCupSize(int gram) = 0;
    virtual void AddWater(int gram, int temperature) = 0;
    virtual void AddSugar(int gram) = 0;
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
    virtual void AddCream(int gram) = 0;
};


enum Cup
{
    Normal,
    Big
};

enum Coffee
{
    Americano,
    Cappuccino
};

class MockSourceOfIngredients : public ISourceOfIngredients
{
public:
    MOCK_METHOD1(SetCupSize, void(int));
    MOCK_METHOD2(AddWater, void(int, int));
    MOCK_METHOD1(AddSugar, void(int));
    MOCK_METHOD1(AddCoffee, void(int));
    MOCK_METHOD1(AddMilk, void(int));
    MOCK_METHOD1(AddMilkFoam, void(int));
    MOCK_METHOD1(AddChocolate, void(int));
    MOCK_METHOD1(AddCream, void(int));
};

class CoffeeMachine
{
public:
    CoffeeMachine(ISourceOfIngredients& source) : m_source(source)
    {

    }
    void CreateCoffee(const Cup cup, const Coffee coffee)
    {
        int volume = 100;

        if (cup == Cup::Big)
        {
            volume = 140;
        }

        m_source.SetCupSize(volume);

        switch (coffee) {
        case Americano:
            americanoRecepie(volume);
            break;
        case Cappuccino:
            cappuccinoRecepie(volume);
            break;
        }
    }

private:
    void americanoRecepie(int volume)
    {
        m_source.AddCoffee(volume / 4 * 3);
        m_source.AddWater(volume / 4, 60);
    }

    void cappuccinoRecepie(int volume)
    {
        m_source.AddCoffee(volume / 3);
        m_source.AddMilk(volume / 3);
        m_source.AddMilkFoam(volume / 3);
        m_source.AddWater(volume - int(volume / 3) * 3, 80);
    }

private:
    ISourceOfIngredients& m_source;
};


// Architecture
// Class CoffeMachine
// Class-Mock SourceOfIngredients

// - americano: water & coffee 1:2 or 1:3. Water temp 60C

// Tests list:
// 1. americano + 100 gram = 1 coffe
// 2. americano + 140 gram = 1 large coffee
// 3. AddCoffee, SetCupSize and AddWater calls once
// 4. Check parameters
// 5. Same for each recipe

TEST(CoffeeMachine, CoffemachineIsHere)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);
}

//- americano: water & coffee 1:3 Water temp 60C
TEST(CoffeeMachine, Americano)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(75)).Times(1);
    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddWater(25, 60)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Americano);
}

//- big 140 gram
//- americano: water & coffee 1:3 Water temp 60C
TEST(CoffeeMachine, AmericanoLargeCup)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, AddCoffee(105)).Times(1);
    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddWater(35, 60)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Americano);
}

//- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
TEST(CoffeeMachine, CappuccinoSmallCup)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddCoffee(33)).Times(1);
    EXPECT_CALL(si, AddMilk(33)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(33)).Times(1);
    EXPECT_CALL(si, AddWater(1, 80)).Times(1);

    cm.CreateCoffee(Cup::Normal, Coffee::Cappuccino);
}

//- big 140 gram
//- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
TEST(CoffeeMachine, CappuccinoLargeCup)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(140)).Times(1);
    EXPECT_CALL(si, AddCoffee(46)).Times(1);
    EXPECT_CALL(si, AddMilk(46)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(46)).Times(1);
    EXPECT_CALL(si, AddWater(2, 80)).Times(1);

    cm.CreateCoffee(Cup::Big, Coffee::Cappuccino);
}

//- latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
TEST(CoffeeMachine, LatteSmallCup)
{
    MockSourceOfIngredients si;
    CoffeeMachine cm(si);

    EXPECT_CALL(si, SetCupSize(100)).Times(1);
    EXPECT_CALL(si, AddCoffee(50)).Times(1);
    EXPECT_CALL(si, AddMilk(25)).Times(1);
    EXPECT_CALL(si, AddMilkFoam(25)).Times(1);
    //no room for water :(

    cm.CreateCoffee(Cup::Normal, Coffee::Latte);
}
