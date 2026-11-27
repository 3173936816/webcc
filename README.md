# 项目说明

一个仿写 egg 框架的 C++ Web 框架。

# 使用说明

`template/` 是项目模板源，`gen_webcc_structure.py` 遍历它、替换占位符并输出到目标目录：

    python3 gen_webcc_structure.py <target-dir> [--name <project-name>]

- 目标目录必须不存在；已存在时报错退出，不覆盖。
- 目录结构由 `template/` 目录本身定义，增删目录/文件只改 `template/`，脚本无需改动。
- 空目录用 `.gitkeep` 占位（生成时跳过该文件，只保留目录）。
- 当前 `template/` 只提供空的目录骨架，不生成任何文件；后续在 `template/` 里放入文件即可一并生成。
- 模板文件中的 `{{name}}` 渲染为项目名（默认取目标目录名）；出现未提供的占位符会报错。
- 除 app/source/ 外，其余代码目录只放 .hpp 头文件；app/source/ 放 .cc/.cpp 实现文件。

# 项目结构说明

<target>/
├── app/                                  应用核心代码，框架的约定目录
│   ├── controller/                      控制器：接收请求、校验参数、调用 service、返回响应
│   ├── middleware/                      中间件：请求进入 controller 前的拦截处理
│   ├── public/                          静态资源根目录，原样对外提供，不走路由
│   │   ├── css/                        样式文件
│   │   ├── js/                         前端脚本
│   │   └── image/                      图片资源
│   ├── schedule/                        定时任务：按周期执行
│   ├── service/                         业务逻辑层：可复用、可被多个 controller 调用的服务
│   ├── view/                            模板文件，服务端渲染页面
│   ├── router/                          路由定义，把 URL 映射到 controller 方法
│   ├── extend/                          扩展目录：按类型扩展框架内置对象
│   └── source/                          实现文件目录：放 .cc/.cpp，对应上面各目录的声明
├── config/                               配置目录，按环境分层
├── tests/                                测试目录
├── modules/                              自定义业务模块/子包，按领域拆分，非强制约定
├── deps/                                 第三方依赖源码或安装脚本（如 C++ 库、预编译产物）
├── logs/                                 运行时日志输出目录，由日志库写入，通常不入库
└── run/                                  运行时产物目录（pid 文件、临时文件），通常不入库
