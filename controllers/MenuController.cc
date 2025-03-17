#include "MenuController.h"

void MenuController::getMenuAdmin(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Role> mpRole(db);
    Mapper<Menu> mpMenu(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    auto roleInDb = mpRole.findOne(
        Criteria(Role::Cols::_id, userInDb.getValueOfRole()));
    auto menuInDb = mpMenu.findOne(
        Criteria(Menu::Cols::_id, roleInDb.getValueOfMenu()));
    Json::Value data = generateMenu(mpMenu, menuInDb.getValueOfId());
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void MenuController::getMenu(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto db = app().getDbClient();
    Mapper<Menu> mpMenu(db);
    auto menuInDb = mpMenu.findOne(
        Criteria(Menu::Cols::_id, 5));
    Json::Value data = generateMenu(mpMenu, menuInDb.getValueOfId());
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

Json::Value MenuController::generateMenu(Mapper<Menu> &mp, int32_t id) {
    auto subMenus = mp.orderBy(Menu::Cols::_id, SortOrder::ASC)
        .findBy(Criteria(Menu::Cols::_parent, id));
    Json::Value menuItems(Json::arrayValue);
    for (const auto &subMenu : subMenus) {
        Json::Value menuItem;
        menuItem["label"] = subMenu.getValueOfLabel();
        menuItem["key"] = subMenu.getValueOfKey();
        auto children = generateMenu(mp, subMenu.getValueOfId());
        if (!children.empty()) {
            menuItem["children"] = children;
        }
        menuItems.append(menuItem);
    }
    return menuItems;
}