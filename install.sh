if command -v g++ >/dev/null 2>&1; then
    g++ main.cpp -o clinote
    sudo cp clinote /usr/local/bin/
    sudo chmod +x /usr/local/bin/clinote
else
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        if [ "$ID" = "fedora" ]; then
            sudo dnf install -y gcc-c++ 
        elif [ "$ID" = "ubuntu" ] || [ "$ID" = "debian" ] || [ "$ID" = "mint" ]; then
            sudo apt install -y g++
        elif [ "$ID" = "arch" ] || [ "$ID" = "manjaro" ] || [ "$ID" = "endeavouros" ]; then
            sudo pacman -S --noconfirm gcc
        fi
        
        g++ main.cpp -o clinote
        sudo cp clinote /usr/local/bin/
        sudo chmod +x /usr/local/bin/clinote
    fi #
fi 
