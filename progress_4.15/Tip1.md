### 使用SSH私钥与GitHub建立连接（便于同步文件夹）

#### 1. 生成SSH密钥

- 打开Git Bash：

  - 在文件夹中右键选择 **Open Git Bash here**。

- 生成密钥：

  ```bash
  ssh-keygen -t rsa -C "your_email@example.com"  # 替换为你的GitHub邮箱
  ```

  - 按回车使用默认路径和空密码。

    ![image-20250416123349249](C:\Users\leglon\AppData\Roaming\Typora\typora-user-images\image-20250416123349249.png)

    

#### 2. 复制公钥到GitHub

- 找到公钥文件：

  - 路径：`C:\Users\你的用户名\.ssh\id_rsa.pub`

- 复制公钥内容：

  - 用记事本打开公钥文件，复制全部内容。

    ![image-20250416123413024](C:\Users\leglon\AppData\Roaming\Typora\typora-user-images\image-20250416123413024.png)

    

- 添加到GitHub：

  1. 进入GitHub仓库 → **Settings** → **Deploy keys** → **Add deploy key**。

  2. 粘贴公钥内容，填写标题（可选），勾选 **Allow write access**。

     ![image-20250416123806749](C:\Users\leglon\AppData\Roaming\Typora\typora-user-images\image-20250416123806749.png)

     

#### 3. 配置本地仓库使用SSH

- 获取SSH仓库地址：

  进入GitHub仓库页面 → **Code** → 选择 **SSH** → 复制地址。

- 修改本地仓库远程URL：

  ```bash
  git remote set-url origin git@github.com:用户名/仓库名.git  # 替换为你的SSH地址
  ```



#### 4. 常用命令



| 命令                   | 说明                   |
| ---------------------- | ---------------------- |
| `git add .`            | 添加所有修改到暂存区   |
| `git commit -m "备注"` | 提交修改并添加备注     |
| `git pull`             | 拉取远程仓库最新代码   |
| `git push`             | 推送本地代码到远程仓库 |



### 示例流程

1. 克隆仓库：

   ```bash
   git clone git@github.com:用户名/仓库名.git
   ```

   ![image-20250416123844948](C:\Users\leglon\AppData\Roaming\Typora\typora-user-images\image-20250416123844948.png)

   

2. 修改文件后提交：

   ```bash
   git add .
   git commit -m "更新说明"
   git push
   ```

通过以上步骤，可以安全地使用SSH密钥与GitHub交互，避免每次输入密码。