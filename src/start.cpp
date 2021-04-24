

// void initial_connection()
// {
//     // arg = "1"; Host
//     // arg = "0"; Join

//     if (response_code == 1)
//     {
//         serv = new Server("127.0.0.1");

//         std::string user_code;

//         do
//         {
//             user_code = serv->get();
//             if (user_code == code)
//             {
//                 serv->send("1");
//                 serv->send(user_name);
//                 oponent_name = serv->get();
//                 break;
//             }
//             else
//             {
//                 serv->send("0");
//             }
//         } while (true);
//     }
//     else if (response_code == 2)
//     {
//         client = new Client("127.0.0.1");

//         do
//         {
//             client->send(code);

//             std::string response = client->get();

//             if (response == "1")
//             {
//                 oponent_name = client->get();
//                 client->send(user_name);
//                 break;
//             }
//         } while (true);
//     }

//     response_code = 0;

// }
