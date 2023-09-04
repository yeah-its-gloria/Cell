let interval = 0;

function isLoadingIndicatorOnScreen() // bool
{
    return document.getElementById("loadingIndicator").style.right === "0%";
}

function showLoadingIndicator()
{
    if (isLoadingIndicatorOnScreen())
    {
        return;
    }

    document.getElementById("loadingIndicator").style.right = "0%";
}

function hideLoadingIndicator()
{
    if (!isLoadingIndicatorOnScreen())
    {
        return;
    }

    document.getElementById("loadingIndicator").style.right = "-22rem";
}

function updateLoadingBar()
{
    let bar = document.getElementById("loadingIndicator").contentDocument.getElementById("loadingProgress");
    if (bar.value >= 100)
    {
        bar.value = 0;
        return;
    }

    bar.value += 10;
}

function toggleLoadingIndicator()
{
    if (isLoadingIndicatorOnScreen())
    {
        if (interval !== 0)
        {
            clearInterval(interval);
            interval = 0;
        }

        hideLoadingIndicator();
        return;
    }

    showLoadingIndicator();
    interval = setInterval(updateLoadingBar, 1000);
}

function showModal()
{
    hideLoadingIndicator();

    let modalIFrame       = document.createElement("iframe");
    modalIFrame.id        = "debugModal";
    modalIFrame.className = "debugModal";
    modalIFrame.src       = "/Pages/Modal.html";
    modalIFrame.scrolling = "no";

    modalIFrame.onload = function()
    {
        let title = modalIFrame.contentWindow.document.getElementById("modalTitle");
        title.innerText = "Confirm";

        let content = modalIFrame.contentWindow.document.getElementById("modalContent");
        content.innerText = "Do you wish to close this page?";

        let button1 = modalIFrame.contentWindow.document.getElementById("modalButton1");
        button1.innerText = "Close";
        button1.onclick = function ()
        {
            document.body.childNodes.forEach(element => element.remove());
            document.body.childNodes.forEach(element => element.remove());

            document.body.style.backgroundColor = "#000000";
        }

        let button2 = modalIFrame.contentWindow.document.getElementById("modalButton2");
        button2.innerText = "Cancel";
        button2.onclick = function ()
        {
            document.body.style.background = "#0a0a0a";
            document.body.removeChild(modalIFrame);
        }
    }

    document.body.appendChild(modalIFrame);
    document.body.style.background = "#000000";
}
