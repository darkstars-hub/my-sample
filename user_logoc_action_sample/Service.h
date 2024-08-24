//
// Created by maila on 2024/08/25.
//

#ifndef CPP_SERVICE_H
#define CPP_SERVICE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>

class User {
public:
    std::string GetEmail() const {
        return "";
    }
    uint32_t GetAge() const {
        return 1;
    }
    uint32_t GetId() const {
        return 1;
    }

    void SetId(int i) {

    }

    void SetEmail(std::string basicString) {

    }

    void SetAge(uint32_t i) {

    }
};
class UserModel {
public:
    UserModel() : id(0), email(""), age(0) {}

    UserModel(uint32_t id, const std::string& email, uint32_t age)
            : id(id), email(email), age(age) {}

    uint32_t GetId() const { return id; }
    const std::string& GetEmail() const { return email; }
    uint32_t GetAge() const { return age; }

    void SetId(uint32_t id) { this->id = id; }
    void SetEmail(const std::string& email) { this->email = email; }
    void SetAge(uint32_t age) { this->age = age; }

    // データベースとのやり取りをするためのメソッドを追加
    void SaveToDatabase() {
        // データベースに保存するロジック
    }

    static UserModel LoadFromDatabase(uint32_t id) {
        // データベースから読み込むロジック
        return UserModel(id, "example@example.com", 25); // 仮のデータ
    }

private:
    uint32_t id;
    std::string email;
    uint32_t age;
};

class UserRepository{
};

class UserDto {
public:
    UserDto() {

    }

    int id;
    std::string email;
    uint32_t age;

    // コンストラクタ、ゲッター、セッターなどを追加
    UserDto(int id, const std::string& email, uint32_t age)
            : id(id), email(email), age(age) {}

    // ゲッター、セッターも必要に応じて実装

    std::string ToCSV() const {
        std::ostringstream oss;
        oss << id << "," << email << "," << age;
        return oss.str();
    }

    static UserDto FromCSV(const std::string& csv) {
        std::istringstream iss(csv);
        std::string token;
        int id;
        std::string email;
        uint32_t age;

        std::getline(iss, token, ',');
        id = std::stoi(token);
        std::getline(iss, email, ',');
        std::getline(iss, token, ',');
        age = std::stoul(token);

        return UserDto(id, email, age);
    }
};

class UserConverter {
public:
    static User ConvertFromDto(const UserDto& dto) {
        User user;
        user.SetId(dto.id);
        user.SetEmail(dto.email);
        user.SetAge(dto.age);
        // 他のフィールドも変換
        return user;
    }

    static UserDto ConvertToDto(const User& user) {
        UserDto dto;
        dto.id = user.GetId();
        dto.email = user.GetEmail();
        dto.age = user.GetAge();
        // 他のフィールドも変換
        return dto;
    }
};

class Order {
private:
    int id;
    int userId;

public:
    Order(int id, int userId) : id(id), userId(userId) {}

    int GetUserId() const { return userId; }
    int GetId() const { return id; }
};

class OrderRepository {
public:
    std::vector<Order> GetOrdersByUserId(int userId) {
        // ユーザーIDに基づいて注文を取得するロジック（データベースから取得など）
        std::vector<Order> orders;
        // 例: orders.push_back(Order(1, userId));
        return orders;
    }

    bool DeleteOrdersByUserId(int userId) {
        // ユーザーIDに基づいて注文を削除するロジック（データベースの操作など）
        std::cout << "Deleted orders for user ID: " << userId << std::endl;
        return true;
    }
};


// ルールクラスのインターフェース
class IUserRule {
public:
    virtual bool IsSatisfiedBy(const User& user) const = 0;
    virtual ~IUserRule() = default;
};

// 特定のルールの実装例
class EmailFormatRule : public IUserRule {
public:
    bool IsSatisfiedBy(const User& user) const override {
        // 簡単なメールフォーマットチェック
        return user.GetEmail().find('@') != std::string::npos;
    }
};

class MinimumAgeRule : public IUserRule {
public:
    bool IsSatisfiedBy(const User& user) const override {
        // 年齢が18歳以上かどうかをチェック
        return user.GetAge() >= 18;
    }
};
class UserValidator {
private:
    std::vector<std::unique_ptr<IUserRule>> rules;

public:
    explicit UserValidator(std::vector<std::unique_ptr<IUserRule>> rules) : rules(std::move(rules)) {}

    bool Validate(const User& user) const {
        for (const auto& rule : rules) {
            if (!rule->IsSatisfiedBy(user)) {
                return false;
            }
        }
        return true;
    }
};

class UserRegistrar {
private:
    UserValidator validator;

public:
    UserRegistrar() :validator({
        //std::make_unique<EmailFormatRule>()
    }){
        std::unique_ptr<IUserRule> p = std::make_unique<EmailFormatRule>();
        std::unique_ptr<IUserRule> q = std::make_unique<MinimumAgeRule>();
        std::vector<std::unique_ptr<IUserRule>> v;
        v.push_back(std::move(p) );
        v.push_back(std::move(q) );
        validator = decltype(validator)(std::move(v));
    }

    bool Register(User& user) {
        if (!validator.Validate(user)) {
            // 検証に失敗した場合の処理
            std::cerr << "User registration failed due to validation." << std::endl;
            return false;
        }
        // 登録処理
        std::cout << "User registered successfully." << std::endl;
        return true;
    }
};

class UserModifier {
private:
    UserValidator validator;

public:
    UserModifier() : validator({ }) {
        std::unique_ptr<IUserRule> p = std::make_unique<EmailFormatRule>();
        std::unique_ptr<IUserRule> q = std::make_unique<MinimumAgeRule>();
        std::vector<std::unique_ptr<IUserRule>> v;
        v.push_back(std::move(p) );
        v.push_back(std::move(q) );
        validator = decltype(validator)(std::move(v));
    }

    bool Modify(User& user) {
        if (!validator.Validate(user)) {
            // 検証に失敗した場合の処理
            std::cerr << "User modification failed due to validation." << std::endl;
            return false;
        }
        // 修正処理
        std::cout << "User modified successfully." << std::endl;
        return true;
    }
};

class UserDestroyer {
private:
    OrderRepository& orderRepo;

public:
    UserDestroyer(OrderRepository& repo) : orderRepo(repo) {}

    bool Destroy(User& user) {
        // 依存する注文を確認する
        auto orders = orderRepo.GetOrdersByUserId(user.GetId());

        if (!orders.empty()) {
            std::cerr << "User has associated orders. Deleting orders first." << std::endl;

            // まず注文を削除する
            if (!orderRepo.DeleteOrdersByUserId(user.GetId())) {
                std::cerr << "Failed to delete orders. User deletion aborted." << std::endl;
                return false;
            }
        }

        // 注文削除後、ユーザーを削除する
        std::cout << "User destroyed successfully." << std::endl;
        return true;
    }
};



// トランザクションクラス
class TransactionManager {
public:
    void BeginTransaction() {
        std::cout << "Transaction started." << std::endl;
        // トランザクション開始のロジック
    }

    void Commit() {
        std::cout << "Transaction committed." << std::endl;
        // トランザクションのコミット
    }

    void Rollback() {
        std::cout << "Transaction rolled back." << std::endl;
        // トランザクションのロールバック
    }
};


// ロジックを表現するクラス
class UserDeletionService  {
private:
    OrderRepository& orderRepo;
    TransactionManager& transactionManager;

public:
    UserDeletionService (OrderRepository& repo, TransactionManager& tm)
            : orderRepo(repo), transactionManager(tm) {}

    bool Destroy(User& user) {
        transactionManager.BeginTransaction();

        try {
            auto orders = orderRepo.GetOrdersByUserId(user.GetId());
            if (!orders.empty()) {
                if (!orderRepo.DeleteOrdersByUserId(user.GetId())) {
                    throw std::runtime_error("Failed to delete orders.");
                }
            }

            // ユーザー削除の処理
            std::cout << "User destroyed successfully." << std::endl;

            transactionManager.Commit();
            return true;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            transactionManager.Rollback();
            return false;
        }
    }
};

//

class IUserView {
public:
    virtual void ShowUserDetails(const UserDto& user) = 0;
    virtual void ShowError(const std::string& message) = 0;
    virtual UserDto GetUserInput() = 0;
    virtual ~IUserView() = default;
};
class UserPresenter {
private:
    IUserView& view;
    UserRegistrar& registrar;
    UserModifier& modifier;
    UserDestroyer& destroyer;

public:
    UserPresenter(IUserView& v, UserRegistrar& r, UserModifier& m, UserDestroyer& d)
            : view(v), registrar(r), modifier(m), destroyer(d) {}

    void RegisterUser() {
        UserDto userDto = view.GetUserInput();
        User user = UserConverter::ConvertFromDto(userDto);

        if (registrar.Register(user)) {
            view.ShowUserDetails(UserConverter::ConvertToDto(user));
        } else {
            view.ShowError("Registration failed.");
        }
    }

    void ModifyUser() {
        UserDto userDto = view.GetUserInput();
        User user = UserConverter::ConvertFromDto(userDto);

        if (modifier.Modify(user)) {
            view.ShowUserDetails(UserConverter::ConvertToDto(user));
        } else {
            view.ShowError("Modification failed.");
        }
    }

    void DestroyUser() {
        UserDto userDto = view.GetUserInput();
        User user = UserConverter::ConvertFromDto(userDto);

        if (destroyer.Destroy(user)) {
            view.ShowError("User destroyed successfully.");
        } else {
            view.ShowError("Deletion failed.");
        }
    }
};

class ConsoleUserView : public IUserView {
public:
    void ShowUserDetails(const UserDto& user) override {
        std::cout << "User Details: " << user.ToCSV() << std::endl;
    }

    void ShowError(const std::string& message) override {
        std::cerr << "Error: " << message << std::endl;
    }

    UserDto GetUserInput() override {
        // ユーザーからの入力を取得するロジック
        // ここでは簡単な例としてハードコーディングしています
        return UserDto(1, "example@example.com", 25);
    }
};

//// main関数での使用例
//int main() {
//    OrderRepository orderRepo;
//    TransactionManager transactionManager;
//
//    UserRegistrar registrar;
//    UserModifier modifier;
//    UserDestroyer destroyer(orderRepo);
//
//    ConsoleUserView view;
//    UserPresenter presenter(view, registrar, modifier, destroyer);
//
//    presenter.RegisterUser();
//    presenter.ModifyUser();
//    presenter.DestroyUser();
//
//    return 0;
//}

#endif //CPP_SERVICE_H
