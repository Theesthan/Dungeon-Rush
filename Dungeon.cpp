#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>
#include<windows.h>

using namespace std;

class Monster;

class Floor {
public:
    string description;
    vector<Floor*> exits;
    Monster* monster;

    Floor(const string& desc) : description(desc), monster(nullptr) {}

    void addExit(Floor* floor) {
        exits.push_back(floor);
    }

    void setMonster(Monster* m) {
        monster = m;
    }
};

class Monster {
public:
    string name;
    int hp;
    int attack;
    double critChance;
    string element;
    string weakness;
    string lootName;

    Monster(const string& n, int h, int a, double crit, const string& elem, const string& weak, const string& loot)
        : name(n), hp(h), attack(a), critChance(crit), element(elem), weakness(weak), lootName(loot) {}
};

class Player {
public:
    string name;
    int hp;
    int attack;
    double critChance;
    const int maxHp = 100;
    vector<string> elements;
    int specialAttackCooldown;
    string currentElement;
    string specialMove;

    Player(const string& n, int h, int a, double crit, const string& elem, const string& move)
        : name(n), hp(h), attack(a), critChance(crit), currentElement(elem), specialMove(move), specialAttackCooldown(0) {
        elements.push_back(elem);
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) hp = 0;
        cout << name << " takes " << damage << " damage! HP: " << hp << endl;
        Sleep(1000);
    }

    bool isAlive() {
        return hp > 0;
    }

    int calculateDamage(int baseAttack, const string& monsterElement, const string& monsterWeakness) {
        double effectiveness = 1.0;

        if (currentElement == monsterWeakness) effectiveness = 1.5;
        else if (currentElement == monsterElement) effectiveness = 0.5;

        int damage = baseAttack * effectiveness;

        if ((rand() % 100) < critChance * 100) {
            cout << "Critical hit!" << endl;
            damage *= 2;
        }

        return damage;
    }

    void setElement(const string& elem) {
        if (elements.size() < 6) {
            elements.push_back(elem);
            cout << name << " has chosen the element: " << elem << "!" << endl;
        } else {
            cout << name << " cannot choose more than 6 elements!" << endl;
        }
    }

    void useLoot(int effect) {
        cout << "You found a loot item with a mysterious effect!" << endl;
        Sleep(1000);
        cout << "Do you want to use it? (1 for Yes, 2 for No): ";
        Sleep(1000);
        int choice;
        cin >> choice;
        if (choice == 1) {
            if (effect > 0) {
                attack += effect;
                cout << name << " increased attack by " << effect << "! New Attack: " << attack << endl;
                Sleep(1000);
            } else {
                hp += effect;
                if (hp > maxHp) hp = maxHp;
                cout << name << " adjusted HP by " << effect << "! New HP: " << hp << endl;
                Sleep(1000);
            }
        } else {
            cout << name << " chose not to use the loot." << endl;
        }
    }

    void levelUp() {
        int choice;
        cout << "Choose your upgrade:\n1. Increase attack by 5\n2. Increase crit chance by 1%\n3. Random heal" << endl;
        cin >> choice;
        Sleep(1000);

        switch (choice) {
            case 1:
                attack += 5;
                cout << "Attack increased to " << attack << "!" << endl;
                break;
            case 2:
                critChance += 0.01;
                cout << "Crit chance increased to " << critChance * 100 << "%!" << endl;
                break;
            case 3: {
                int healAmount = spinRoulette();
                heal(healAmount);
                cout << name << " healed for " << healAmount << " HP!" << endl;
                break;
            }
            default:
                cout << "Invalid choice. No upgrade made." << endl;
                break;
        }
    }

    int spinRoulette() {
        return rand() % 101;
    }

    void heal(int amount) {
        hp += amount;
        if (hp > maxHp) {
            hp = maxHp;
        }
    }

    void useSpecialAttack(Monster* monster) {
        if (specialAttackCooldown > 0) {
            cout << "Special attack is on cooldown for " << specialAttackCooldown << " more floors!" << endl;
            return;
        }
        int damage = calculateDamage(attack, monster->element, monster->weakness);
        monster->hp -= damage * 2; // Double damage with crit
        cout << name << " uses special attack on " << monster->name << " for " << damage * 2 << " damage! Monster HP: " << monster->hp << endl;
        specialAttackCooldown = 2; // Set cooldown
    }

    void decreaseCooldown() {
        if (specialAttackCooldown > 0) specialAttackCooldown--;
    }
};

Floor* createFloor(int floorNumber) {
    return new Floor("Floor " + to_string(floorNumber));
}

Monster* createRandomMonster(int floorNumber) {
    string monsterNames[][2] = {
        {"Goblin", "None"}, {"Skeleton", "None"}, {"Orc", "None"}, {"Troll", "None"},
        {"Wraith", "None"}, {"Zombie", "None"}, {"Giant Rat", "None"}, {"Bandit", "None"},
        {"Spider", "None"}, {"Vampire", "None"}, {"Dragon", "None"}, {"Demon", "None"},
        {"Golem", "None"}, {"Chimera", "None"}, {"Manticore", "None"}, {"Hydra", "None"},
        {"Banshee", "None"}, {"Minotaur", "None"}, {"Behemoth", "None"}, {"Kraken", "None"}
    };

    int monsterIndex = (floorNumber - 1) % 20;
    int hp = (rand() % (5 + floorNumber * 5) + 5) * 2;
    int attack = rand() % (3 + floorNumber) + 1;
    double critChance = 0.05;

    string lootNames[] = {
        "Mysterious Stone", "Goblin's Gold", "Skeleton Key", "Troll's Toenail",
        "Wraith's Whisper", "Zombie Flesh", "Giant Rat Fur", "Bandit's Booty",
        "Spider Silk", "Vampire Fang", "Dragon Scale", "Demon Horn",
        "Golem Core", "Chimera Claw", "Manticore Stinger", "Hydra Tooth",
        "Banshee Wail", "Minotaur Horn", "Behemoth Fang", "Kraken Ink"
    };

    string element = "None";
    string weakness = "None";

    if (floorNumber > 20) {
        string elements[] = {"Fire", "Water", "Earth", "Air", "Light", "Shadow"};
        element = elements[rand() % 6];
        weakness = (element == "Fire") ? "Water" : (element == "Water") ? "Earth" :
                   (element == "Earth") ? "Air" : (element == "Air") ? "Fire" :
                   (element == "Light") ? "Shadow" : "Light";
    }

    string lootName = lootNames[monsterIndex % 20];
    return new Monster(monsterNames[monsterIndex][0], hp, attack, critChance, element, weakness, lootName);
}

void battle(Player& player, Monster* monster) {
    cout << "A wild " << monster->name << " appears! ";
    if (monster->element != "None") {
        cout << "Element: " << monster->element << ", Weakness: " << monster->weakness << endl;
    } else {
        cout << "No elemental attributes." << endl;
    }

    while (player.isAlive() && monster->hp > 0) {
        int action;
        cout << "Choose action:\n1. Attack\n2. Special Attack" << endl;
        cin >> action;
        Sleep(2000);

        if (action == 1) {
            int damage = player.calculateDamage(player.attack, monster->element, monster->weakness);
            monster->hp -= damage;
            cout << player.name << " attacks " << monster->name << " for " << damage << " damage! Monster HP: " << monster->hp << endl;

            if (monster->hp > 0) {
                int monsterDamage = (rand() % 100) < monster->critChance * 100 ? monster->attack * 2 : monster->attack;
                player.takeDamage(monsterDamage);
            }
        } else if (action == 2) {
            player.useSpecialAttack(monster);
            if (monster->hp <= 0) break;
        } else {
            cout << "Invalid action. Please choose again." << endl;
        }
    }

    if (player.isAlive()) {
        cout << "You defeated the " << monster->name << "!" << endl;
        Sleep(1000);
        if ((rand() % 100) < 80) {
            cout << monster->lootName << " has been dropped!" << endl;
            player.useLoot(rand() % 3 - 1);
        }
        player.levelUp();
    } else {
        cout << "You were defeated by the " << monster->name << "..." << endl;
    }
}

void exploreDungeon(Player& player) {
    stack<Floor*> floorStack;
    Floor* currentFloor = createFloor(1);
    floorStack.push(currentFloor);

    for (int floorCounter = 1; floorCounter <= 100; ++floorCounter) {
        cout << "\n" << currentFloor->description << endl;
        cout << "Hero Stats - HP: " << player.hp << ", Crit Chance: " << player.critChance * 100 << "%, Attack: " << player.attack << endl;
        Sleep(1000);

        currentFloor->setMonster(createRandomMonster(floorCounter));

        if (currentFloor->monster) {
            battle(player, currentFloor->monster);
            if (!player.isAlive()) break;
        }

        if (floorCounter % 20 == 0 && player.isAlive()) {
            player.levelUp();
        }

        Floor* newFloor = createFloor(floorCounter + 1);
        currentFloor->addExit(newFloor);
        floorStack.push(newFloor);
        currentFloor = newFloor;

        if (floorCounter % 5 == 0 && floorCounter < 100) {
            currentFloor->setMonster(new Monster("Boss Monster", 100 + (floorCounter * 20), 20 + floorCounter, 0.1, "Fire", "Water", "Boss Loot"));
        }

        player.decreaseCooldown();
    }

    while (!floorStack.empty()) {
        delete floorStack.top();
        floorStack.pop();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    cout << "Choose your special move:\n1. Fire Strike\n2. Water Wave\n3. Earth Quake\n4. Air Slash\n5. Light Beam\n6. Shadow Cloak" << endl;
    Sleep(1000);
    int moveChoice;
    cin >> moveChoice;

    string specialMove;
    string element;
    switch (moveChoice) {
        case 1: specialMove = "Fire Strike"; element = "Fire"; break;
        case 2: specialMove = "Water Wave"; element = "Water"; break;
        case 3: specialMove = "Earth Quake"; element = "Earth"; break;
        case 4: specialMove = "Air Slash"; element = "Air"; break;
        case 5: specialMove = "Light Beam"; element = "Light"; break;
        case 6: specialMove = "Shadow Cloak"; element = "Shadow"; break;
        default: specialMove = "Normal Attack"; element = "None"; break;
    }

    Player player("Hero", 80, 8, 0.5, element, specialMove);
    exploreDungeon(player);

    return 0;
}