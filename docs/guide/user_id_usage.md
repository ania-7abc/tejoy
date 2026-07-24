# UserId usage

This example shows the use of the
class [UserId](https://github.com/ania-7abc/tejoy/blob/main/include/tejoy/user.hpp#L28-L44).
It is used for [asymmetric encryption and decryption](https://en.wikipedia.org/wiki/Public-key_cryptography)

## Constructing UserId

UserId has the following constructors:

| Constructor signature                                    | Remark                                                                       |
|----------------------------------------------------------|------------------------------------------------------------------------------|
| `UserId(std::string public_key, std::string secret_key)` | The object created by this constructor can both encrypt and decrypt messages |
| `UserId(std::string public_key)`                         | The object created by this constructor cannot encrypt or decrypt messages    |
| `UserId::generate()`                                     | This is a method to generate a new unique UserId                             |

!!! info

    You can find the API for UserId on [this page](../api/tejoy/UserId)

When constructing UserId in the second way ^?^{title="Only the public key is passed"} the class cannot encrypt and
decrypt messages. It is only needed to pass it to `encrypt` (as a recipient) or to `decrypt` (as
sender)

Examples of UserId construction:

=== "UserId::generate"

    ```cpp
    auto alice = tejoy::UserId::generate();
    auto bob = tejoy::UserId::generate();
    ```

=== "UserId(std::string public_key, std::string secret_key)"

    ```cpp
    auto alice = tejoy::UserId(alice_pk, alice_sk);
    auto bob = tejoy::UserId(bob_pk, bob_sk);
    ```

## Obtaining public and private keys

If you need to get 1 of the keys you can use these methods:

=== "Public key"

    ```cpp
    std::cout << alice.get_public_key() << '\n';
    ```

=== "Secret key"

    ```cpp
    if (bob.has_secret_key())
        std::cout << bob.get_secret_key() << '\n';
    ```

!!! info

    Keys are byte arrays encoded in Base64

!!! note

    The public key is unique, so it is used in the messenger as a user ID

!!! warning

    The secret key must be kept completely confidential and not shown to anyone

## Encrypting and decrypting messages

```cpp hl_lines="6-7"
std::string message = "Hello, World!";

std::string cypher = alice.encrypt(message, bob); // (1)!
std::string out = bob.decrypt(cypher, alice);

assert(out == message);
```

1. When encrypting, the recipient is passed to the encrypt function
   Only someone who has the recipient's secret key (usually only the recipient himself) can decrypt the message

!!! info

    The message can only be decrypted by the recipient (second argument encrypt). To do this, it must have the sender's UserId created by the first constructor  
    [See also](../api/tejoy/UserId#encrypt)

*[UserId]: Article theme
