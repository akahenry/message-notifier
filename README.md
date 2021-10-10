# message-notifier

Message-notifier is a terminal based application for messages like Twitter. It works with messages which are send to all the followers of the user. 

## Requirements
- Git
- Make
- C++17
- g++

## How to install

1. Clone the repository
```
$ git clone git@github.com:akahenry/message-notifier
```
2. Compile the applications with make
```
$ make all
```

Now the binaries should be in `bin` folder. 

## How to use

### Client
The client needs to know your username, the server address and port. So you must pass them into its parameters:

```
$ ./bin/client john 127.0.0.1 3000
```

You can send messages with the `SEND` prefix:

```
$ ./bin/client john 127.0.0.1 3000
> SEND Hello world!
```

You can follow other users with the `FOLLOW @` prefix:

```
$ ./bin/client john 127.0.0.1 3000
> FOLLOW @peter
```

You can also exit the application with `EXIT` command or `CTRL+C` or `CTRL+D` (exit):
```
$ ./bin/client john 127.0.0.1 3000
> EXIT
```

### Server
The server needs to know the port which it will listen for client connections. As the client, you must pass it into its parameters.

```
$ ./bin/server 3000
```