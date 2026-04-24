#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

IncomeTaxPlot::IncomeTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

int IncomeTaxPlot::getPercentage() const {
    return percentage;
}

void IncomeTaxPlot::startEvent(PlotContext& ctx) {
    //TODO: gunakan formatter untuk menampilkan pesan
    std::string msg = "Pilihan (1/2)"; //dummy
    std::string answer;
    while (true){
        answer = CommandHandler::promptInput(msg);
        if (answer.compare("1") || answer.compare("2")){
            break;
        }
        else{
            std::cout << InvalidInputException().what();
            //TODO: gunakan formatter untuk tampilkan pesan error
        }
    }

    if (answer.compare("1")){
        ctx.getCurrentPlayer().pay(FLAT); //TODO: gunakan payTaxes atau tangani bankrupt
    }

    else if (answer.compare("2")){
        ctx.getCurrentPlayer().pay(ctx.getCurrentPlayer().getTotalWealth() * PPH/100);//TODO: gunakan payTaxes atau tangani bankrupt
    }
}