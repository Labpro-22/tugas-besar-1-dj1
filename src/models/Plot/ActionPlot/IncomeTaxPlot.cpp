#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

#include <iostream>
#include "core/GameException.hpp"
#include "core/services/CommandHandler.hpp"
#include "models/Player/Player.hpp"

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
        if (answer == "1" || answer == "2"){
            break;
        }
        else{
            std::cout << InvalidInputException().what();
            //TODO: gunakan formatter untuk tampilkan pesan error
        }
    }

    if (answer == "1"){
        ctx.getCurrentPlayer().payTaxes(FLAT);
    }

    else if (answer == "2"){
        ctx.getCurrentPlayer().payTaxes(ctx.getCurrentPlayer().getTotalWealth() * PPH/100);
    }
}