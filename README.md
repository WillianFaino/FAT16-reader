# Sistema de Arquivos FAT em C++

Este projeto em C++ implementa uma ferramenta para ler e interpretar imagens de disco formatadas no sistema FAT (File Allocation Table). Ele extrai informações detalhadas sobre a estrutura do disco, incluindo o Boot Record, Root Directory e o conteúdo de arquivos válidos na imagem. A ferramenta foi projetada para funcionar com imagens de disquete (`floppyfat2.img`).

## Funcionalidades

1. **Leitura do Boot Record**: Exibe os metadados da imagem, contendo detalhes críticos do sistema de arquivos.
2. **Identificação das Regiões FAT, Root Directory e Data**: Localiza as áreas FAT, Root Directory e Data na imagem.
3. **Exibição de Entradas Válidas no Root Directory**: Lista arquivos e diretórios válidos presentes no Root Directory.
4. **Leitura e Exibição do Conteúdo de Arquivos**: Mostra o conteúdo dos arquivos válidos na imagem de disco.

## Estrutura

- **`FileSystem.h`**: Cabeçalho com a definição da classe `FileSystem`, responsável pelas operações de leitura e exibição da imagem FAT.
- **`main.cpp`**: Arquivo principal para entrada do nome da imagem e execução dos métodos de `FileSystem`.

## Como Executar

1. Compile o programa em C++.
2. Execute e insira o nome da imagem FAT, por exemplo, `floppyfat2.img`.
3. O programa irá processar e exibir informações sobre o Boot Record, Root Directory, além do conteúdo dos arquivos.

## Exemplo de Saída

- **Informações do Boot Record**
- **Localização das Regiões FAT, Root Directory e Data**
- **Listagem de Arquivos e Diretórios Válidos no Root Directory**
- **Conteúdo dos Arquivos**

### Licença

Distribuído sob a licença MIT.
