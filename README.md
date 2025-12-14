# minishell

**Author:** Yeray Alcala Paz

## Dependencies

There are lot of ways of compiling the project. Even though, we advise you use the `g++` compiler.

Ubuntu:

```bash
sudo apt install build-essential
```

Arch linux:

```bash
sudo pacman -Sy gcc
```

## Compiling project

Compiling the project is as easy as compiling any other C++ project, you just have to make sure you do it correctly. Here are the steps:

1. First of all, make sure your current working directory is the root of the project. Specifically, you should be in the directory where `minishell.cpp` file and `utils` folder are.

2. Ensure all dependencies are installed.

3. Execute the following command:

```bash
g++ -Iutils minishell.cpp utils/*.cpp -o minishell
```

4. A file named `minishell` should have been created. Ensure it has execute permissions. If not, grant the file execute permissions with

```bash
chmod u+x minishell
```

5. Choose how you would like to run the program:

    1. For interactive shell, execute

    ```bash
    ./minishell
    ```

    2. For executing a script, execute

    ```bash
    ./minishell path/to/script
    ```


## Minishell modes

### Interactive mode

Using this mode is a wonderful idea if you want to execute commands one-by-one. Once in, the program will wait for your input. From now on you only need to type commands (and press Enter, of course) as you would usually do in a shell.

Once finished, you can exit the minishell by typing

```
salir
```
and pressing "Enter" key.

### Command file (script)

Although it's a basic feature, you can create a file that contains commands (one command at a line) and execute it using the minishell. You only need to execute

```bash
./minishell path/to/file
```
and let the magic do its work. Any script can exit inmediately by using command

```
salir
```
If not, whole script will be executed until EOF.


## Characteristics

Here's a list of all the things that can be used in either interactive mode or command file mode:

- **Prompt improved**: Your current working directory is shown in the promp.

- **Multi-command:** You can execute multiple command in just a line! You only have to write a __;__ between every command. Example:

```
command1 ; command2 ; command3 ; command4
```

- **Comments:** Want to write something that shouldn't be executed? You can create a comment using __#__ symbol. Note than everything in the same lime after a __#__ will become a comment.

- **Redirections:** A way of changing default file descriptors.

    - `command < /path/to/in`: Changes the input file. The command will read data from this file.

    - `command > /path/to/out`: Changes the output file. All lines will be written in this file. If the file exists, it overwrites its content. If it doesn't exist, it is created with the output.

    - `command >> /path/to/append`: Same as out, but if file exists, output data is appended at the end of the file.

    - `command 2> /path/to/err`: Changes the error file. If command throws an error, it will be written in the file. If file exists, it is overwritten. If not, it is created with the appropiate content.

**Note**: A single command can contain multiple redirections. Take into account that, if multiple redirections of the same type are written, only the last of them will be taken into account.

- **Pipes:** Makes the output of a command be the input of the next command. Currently this can only be used with two commands. Use:

```
command1 | command2
```

**Note**: Redirections also work with this, although may not work as expected.

- **cd:** This command allows changing the current working directory. These are the ways of using it:

    - **No args:** Your working directory is set to your home path.

    - **~**: Same as no args. Your working directory is set to your home path.

    - **.**: The actual directory.

    - **..**: Parent directory.

    - **/path/to/dir**: Sets your working directory to that dir.

- **Scripts:** You can organize your commands in a file called _Script_ which can be executed. Lines/Commands in the script will be executed sequentially.

## Support

If further assistance is required, please consider contacting me through e-mail: <yealcala@unirioja.es>